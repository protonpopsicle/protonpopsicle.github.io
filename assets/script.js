$(function() {
    var toTheme = $('body').data('to-theme');
    if (toTheme !== undefined) {
	setTimeout(function() {
	    $('body').attr('class', toTheme);
	}, 2000);
    }
});
