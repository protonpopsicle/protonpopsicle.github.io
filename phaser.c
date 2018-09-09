#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fts.h>


int process_file(char *path);
FILE *out_fp;

void process_line(char *line, ssize_t linelen) {
	int i = 0;
	int expr_found = 0;
	char buf[linelen];
	ssize_t buflen = 0;

	for (; i < linelen; i++) {
		if (!expr_found && line[i] == '+' && line[i + 1] == '+') {
			expr_found = 1;
			i += 2;
		}
		if (expr_found && line[i] == '-' && line[i + 1] == '-') {
			i += 2;
			break;
		}
		if (!expr_found) {
			fputc(line[i], out_fp);
		} else {
			buf[buflen] = line[i];
			buflen++;
		}
	}
	if (buflen) {
		buf[buflen] = '\0';
		process_file(buf);
	}
	for (; i < linelen; i++) {
		fputc(line[i], out_fp);
	}
}

int process_file(char *path) {
	FILE *fp = fopen(path, "r");
	if (fp == NULL) {
		return -1;
	}
	printf("  %s\n", path);

	char *line = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	while ((linelen = getline(&line, &linecap, fp)) > 0) {
		process_line(line, linelen);
	}
	fclose(fp);
	return 0;
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("usage: phaser <path>\n");
		return -1;
	}

	FTS *ftsp;
	FTSENT *p;
	int fts_options = FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR;

	char *paths[] = {argv[1], NULL};
	ftsp = fts_open(paths, fts_options, NULL);
	if (ftsp == NULL) {
		printf("Error traversing file hierarchy: %s\n", strerror(errno));
		return -1;
	}

	while(1) {
		p = fts_read(ftsp);
		if (p == NULL) {
			break;
		}

		/* printf("%s\n", p->fts_name); */
		/* printf("  path: %s\n", p->fts_path); */
		/* printf("  level: %d\n", p->fts_level); */
		/* printf("  parent: %s\n", p->fts_parent->fts_name); */

		char *ext = strrchr(p->fts_path, '.');
		if (ext && strcmp(ext + 1, "src") == 0) {

			// derive output path
			size_t outlen = p->fts_pathlen + 1;
			char outname[outlen];
			strcpy(outname, strchr(p->fts_path, '/') + 1);
			strcpy(strrchr(outname, '.') + 1, "html");

			// create subdirectories
			if (p->fts_level > 1) {
				char dirname[outlen];
				strcpy(dirname, outname);
				char *ptr = strrchr(dirname, '/');
				*ptr = '\0';
				char buffer[outlen + 9];
 				strcpy(buffer, "mkdir -p ");
				strcat(buffer, dirname);
				system(buffer);
			}
			out_fp = fopen(outname, "w");
			if (out_fp == NULL) {
				printf("Could not open file for writing: %s\n", strerror(errno));
				fts_close(ftsp);
				return -1;
			}
			printf("writing: %s\n", outname);
			printf("scanning:\n");
			if (process_file(p->fts_path) < 0) {
				printf("Critical error: %s\n", strerror(errno));
				fclose(out_fp);
				fts_close(ftsp);
				return -1;
			}
			fclose(out_fp);
		}
	}
	fts_close(ftsp);
	return 0;
}
