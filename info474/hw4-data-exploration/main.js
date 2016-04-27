/// <reference path="./typings/d3/d3.d.ts" />
$(function() {
    var cachedData;

    var timeSeriesChart = timeSeries()
        .width(640)
        .height(480);

    // Load Burke-Gilman trail data.
    d3.csv('burke_gilman_trail_counters.csv', function(data) {
        cachedData = data;

        var timeSeriesDiv = d3.select('#time-series')
            .datum(data)
            .call(timeSeriesChart);
    });
});