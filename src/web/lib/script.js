var selected = null;

function change(id, name, url) {
    document.getElementById("h").innerHTML = name;
    if (id >= 0) {
        if (selected) $(selected).css("font-weight","normal");
        selected = $('li')[id];
        $(selected).css("font-weight","bold");

    } else {
        if (selected) $(selected).css("font-weight","normal");
        selected = null;
    }
    showGraph(id, url);
}

function showGraph(id, url) {
    $.ajax(url, {
        success: function(data) {
            if (!g) renderGraph(prepareData(id, data));
            else updateData(prepareData(id, data));
        }
    });
}

function prepareData(id, data) {
    var rows = data.split('\n');
    var matrix = [];
    rows.forEach(function(row) {
        matrix.push(row.split(' '));
    });
    var series = [];
    var x = 0;
    if (id < 0) {
        for (var i = 0; i < matrix[0].length; i++) {
            var s = [x++];
            for (var y = 0; y < matrix.length; y++) {
                if (y == y) s.push(matrix[y][i]);
            }
            series.push(s);
        }
    } else {
        for (var i = 0; i < matrix[0].length; i++) {
            var s = [x++];
            s.push(matrix[id][i]);
            series.push(s);
        }
    }
    return series;
}

var g;

function renderGraph(data) {
    var labels = [];
    var i = 0;
    data[0].forEach(function(v) {
        labels.push(i++)
    });
    g = new Dygraph(document.getElementById("div_g"), data, {
        labels: labels,
        animatedZooms: true
    });
}

function updateData(data) {
    var labels = [];
    var i = 0;
    data[0].forEach(function(v) {
        labels.push(i++)
    });
    g.updateOptions( { 'file': data, labels: labels } );
}


$(document).ready(function() {
    showGraph(-1, 'n_spectrum.txt');
    autoResizeDiv();
});

function autoResizeDiv()
{
    $('.fit').height(window.innerHeight);
}
window.onresize = autoResizeDiv;