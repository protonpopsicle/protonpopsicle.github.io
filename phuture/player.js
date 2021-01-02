var tracks = []
var trackMarks = document.getElementById("tracks").children;
var playHead = document.getElementById("play-head");

var theme = window.location.search.substring(3)
if (theme.length > 2) {
    document.getElementsByTagName("svg")[0].classList.add(theme);
}

function convert(input) {
    var parts = input.split(':'),
        minutes = +parts[0],
        seconds = +parts[1];
    return (minutes * 60 + seconds);
};

var clickHandler = function() {
    for (var i = 0; i < tracks.length; i++) {
	tracks[i].classList.remove("playing");
    }
    this.classList.add("playing");
    var title = this.children[0].textContent;
    var length = this.getAttribute("data-length");
    rotate(playHead, this.getAttribute("data-deg"));
    document.getElementById("track-title").textContent = title + " (" + length + ")";
};

var rotate = function(elem, deg) {
    elem.style.transform = 'rotate('+deg+'deg)';
};

var totalLength = 0;
var prevLength = 0;

for (var i = 0; i < trackMarks.length; i++) {
    var length = convert(trackMarks[i].getAttribute("data-length"));
    totalLength += length;
}

for (var i = 0; i < trackMarks.length; i++) {
    var elem = trackMarks[i];
    var length = convert(elem.getAttribute("data-length"));
    var deg = Math.round((prevLength/totalLength) * 360);
    elem.setAttribute("data-deg", deg);
    rotate(elem, deg);
    tracks.push(elem);
    elem.addEventListener('click', clickHandler, false);
    prevLength += length;
}
