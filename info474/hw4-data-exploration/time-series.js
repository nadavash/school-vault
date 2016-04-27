/// <reference path="./typings/d3/d3.d.ts" />

// Returns a time series chart function that can be used to generate a reusable
// time series svg visualization.
function timeSeries() {
    var TRANSITION_DURATION = 1500;
    var data = [];
    var width = 640;
    var height = 480;
    var backgroundColor = 'white';
    var padding = {
        top: 50,
        bottom: 50,
        left: 50,
        right: 50
    };

    var xScale, yScale;

    var updateData;

    // Chart generation closure.
    var chart = function(selection) {
        selection.each(function() {
            var svg = d3.select(this).append('svg')
                .attr('width', width)
                .attr('height', height)
                .style('background-color', backgroundColor);

            var pointsContainer = svg.append('g')
                .attr('transform', 'translate(' + padding.left + ',' + padding.top + ')');

            var valueLinePath = pointsContainer.append('path')
                .attr('class', 'line');

            var xAxisLabel = svg.append('g')
                .attr('class', 'axis')
                .attr('transform', 'translate(' + padding.left + ',' + (height - padding.top) + ')');

            var yAxisLabel = svg.append('g')
                .attr('class', 'axis')
                .attr('transform', 'translate(' + padding.left + ',' + padding.top + ')');

            var xAxisText = svg.append('text')
                .attr('transform', 'translate(' + (padding.left + innerWidth()/2) + ',' + (innerHeight() + padding.top + 30) + ')')
                .attr('class', 'title')
                .text('time');

            var yAxisText = svg.append('text')
                .attr('transform', 'translate(' + (padding.left - 30) + ',' + (height/2) + ') rotate(-90)')
                .attr('class', 'title')
                .text('counts');

            updateData = function() {
                setScales(data);
                setAxes(xAxisLabel, yAxisLabel);
                var adjustedWidth = innerWidth();
                var adjustedHeight = innerHeight();

                var points = pointsContainer.selectAll('circle').data(data);
                points.enter().append('circle')
                    .attr('r', 3)
                    .attr('cx', function(d) { return xScale(d.date); })
                    .attr('cy', function(d) { return yScale(d.total); })
                    .style('opacity', 0.7)
                    .attr('title', function(d) { return d.time; });

                points.exit().remove();

                points.transition()
                    .attr('cx', function(d) { return xScale(d.date); })
                    .attr('cy', function(d) { return yScale(d.total); })
                    .style('opacity', 0.7)
                    .attr('title', function(d) { return d.time; });

                valueLinePath
                    .transition()
                    .attr('d', valueLine(data));
            };

            updateData();
        });
    };

    // Gets/sets the data associated with this chart.
    chart.data = function(val) {
        if (!arguments.length) return data;
        data = val;
        if (typeof updateData === 'function') {
            updateData();
        }
    };

    // Gets/sets the width of this chart.
    chart.width = function(val) {
        if (!arguments.length) return width;

        width = val;
        return this;
    };

    // Gets/sets the height of this chart.
    chart.height = function(val) {
        if (!arguments.length) return height;

        height = val;
        return this;
    };

    // Gets/sets the background color of this chart.
    chart.backgroundColor = function(val) {
        if (!arguments.length) return backgroundColor;

        backgroundColor = val;
        return this;
    }

    // Returns the width of the chart, excluding the paddings.
    var innerWidth = function() {
        return width - padding.left - padding.right;
    };

    // Returns the height of the chart, excluding the paddings.
    var innerHeight = function() {
        return height - padding.top - padding.bottom;
    };

    var setScales = function(data) {
        xScale = d3.time.scale()
            .domain([data[0].date, data[data.length - 1].date])
            .range([0, innerWidth()]);

        var yMin = d3.min(data, function(d) {
            return +d.total;
            // return Math.min(d.ped_north, d.ped_south,
            //                 d.bike_north, d.bike_south);
        });
        var yMax = d3.max(data, function(d) {
            return +d.total;
            // return Math.max(d.ped_north, d.ped_south,
            //                 d.bike_north, d.bike_south);
        });
        yScale = d3.scale.linear()
            .domain([yMin, yMax])
            .range([innerHeight(), 0]);
    };

    var setAxes = function(xAxisLabel, yAxisLabel) {
        var xAxis = d3.svg.axis()
            .scale(xScale)
            .orient('bottom');

        var yAxis = d3.svg.axis()
            .scale(yScale)
            .orient('left');

        xAxisLabel.transition().duration(TRANSITION_DURATION).call(xAxis);

        yAxisLabel.transition().duration(TRANSITION_DURATION).call(yAxis);
    };

    var valueLine = d3.svg.line()
        .x(function (d) { return xScale(d.date); })
        .y(function (d) { return yScale(d.total); });

    return chart;
}