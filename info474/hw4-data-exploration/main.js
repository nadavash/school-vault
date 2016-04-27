/// <reference path="./typings/d3/d3.d.ts" />
$(function() {
    var START_DATE = Date.parse('2014-01-01');

    var cachedData;

    var timeSeriesChart = timeSeries()
        .width(640)
        .height(480);

    // Load Burke-Gilman trail data.
    d3.csv('burke_gilman_trail_counters.csv', function(data) {
        cachedData = data;

        timeSeriesChart.data(data.slice(0, 24).map(function(val) {
            val.date = Date.parse(val.date);
            return val;
        }));
        var timeSeriesDiv = d3.select('#time-series')
            .call(timeSeriesChart);
    });

    $('[name=data-day]').change(function() {
        var selectedDate = Date.parse($('[name=data-day]').val());
        var timeDiff = Math.abs(selectedDate.getTime() - START_DATE.getTime());
        var diffHours = Math.ceil(timeDiff / (1000 * 3600));
        console.log(diffHours);
        timeSeriesChart.data(cachedData.slice(diffHours, diffHours + 24)
            .map(function(val) {
                val.date = Date.parse(val.date);
                return val;
            }));
    });
});
