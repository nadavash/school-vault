/// <reference path="./typings/d3/d3.d.ts" />

// Returns a time series chart function that can be used to generate a reusable
// time series svg visualization.
function timeSeries() {
    var data = [];
    var width = 640;
    var height = 480;
    var backgroundColor = '#DDDDDD';
    var margin = {
        top: 50,
        bottom: 100,
        left: 70,
        right: 50
    };

    var xScale, yScale;

    var updateData;

    var chart = function(selection) {
        selection.each(function(data) {
            var svg = d3.select(this).append('svg')
                .attr('width', width)
                .attr('height', height)
                .style('background-color', backgroundColor);

            var rectsContainer = svg.append('g')
                .attr('transform', 'translate(' + margin.left + ',' + margin.top + ')');

            var xAxisLabel = svg.append('g')
                .attr('class', 'axis')
                .attr('transform', 'translate(' + margin.left + ',' + (height + margin.top) + ')');

            var yAxisLabel = svg.append('g')
                .attr('class', 'axis')
                .attr('transform', 'translate(' + margin.left + ',' + margin.top + ')');

            var xAxisText = svg.append('text')
                .attr('transform', 'translate(' + (width/2) + ',' + (height - margin.top) + ')')
                .attr('class', 'title')
                .text('time');

            var yAxisText = svg.append('text')
                .attr('transform', 'translate(' + (margin.left - 30) + ',' + (margin.top + height/2) + ') rotate(-90)')
                .attr('class', 'title')
                .text('counts');

            updateData = function() {
                setScales(data);
            };
        });
    };

    chart.data = function(val) {
        if (!arguments.length) return data;
        data = val;
        if (typeof updateData === 'function') {
            updateData();
        }

    };

    chart.width = function(val) {
        if (!arguments.length) return width;

        width = val;
        return this;
    };

    chart.height = function(val) {
        if (!arguments.length) return height;

        height = val;
        return this;
    };

    chart.backgroundColor = function(val) {
        if (!arguments.length) return backgroundColor;

        backgroundColor = val;
        return this;
    }

    // Returns the width of the chart, excluding the margins.
    var innerWidth = function() {
        return width - margin.left - margin.right;
    };

    // Returns the height of the chart, excluding the margins.
    var innerHeight = function() {
        return height - margin.top - margin.bottom;
    };

    var setScales = function(data) {
        xScale = d3.time.scale()
            .domain([data[0].date, data[data.length - 1].date])
            .range([0, width]);

        var yMin = d3.min(data, function(d) {
            return Math.min(d.ped_north, d.ped_south,
                            d.bike_north, d.bike_south);
        });
        var yMax = d3.max(data, function(d) {
            return Math.max(d.ped_north, d.ped_south,
                            d.bike_north, d.bike_south);
        });
        yScale = d3.scale.linear()
            .domain([yMin, yMax])
            .range([height, 0]);
    };

    return chart;
}