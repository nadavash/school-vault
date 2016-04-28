/// <reference path="./typings/d3/d3.d.ts" />
$(function() {
    var START_DATE = Date.parse('2014-01-01');

    var cachedData;

    var timeSeriesChart = timeSeries()
        .width(800)
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

        $('circle')
            .tooltip({
                container: 'body',
                placement: 'top'
            });
    });

    $('[name=data-day]').change(updateData);
    $('[name=data-num-days]').change(updateData);

    function updateData() {
        console.log('hi');
        var selectedDate = Date.parse($('[name=data-day]').val());
        var numDays = $('[name=data-num-days]').val();

        $('#num-days').html(numDays);

        var timeDiff = Math.abs(selectedDate.getTime() - START_DATE.getTime());
        var diffHours = Math.ceil(timeDiff / (1000 * 3600));
        if (diffHours % 2 != 0) {
            diffHours += 1;
        }
        timeSeriesChart.data(cachedData.slice(diffHours, diffHours + numDays * 24)
            .map(function(val) {
                val.date = Date.parse(val.date);
                return val;
            }));
    };
});
