/// <reference path="../typings/d3/d3.d.ts" />

// Returns a heatmap chart function that can be used to generate a reusable
// heatmap for a data table.
function heatmap() {
    // User-editable attributes
    var width = 640,
        height = 480,
        minColor = d3.rgb('white'),
        maxColor = d3.rgb('red'),
        xLabels = null,
        yLabels = null,
        transitionDuration = 750,
        rowLabelName = 'Name';

    // Positioning/space attributes
    var margin = {
        left: 100,
        right: 50,
        top: 50,
        bottom: 50
    };

    // Scales
    var xScale = d3.scale.ordinal(),
        yScale = d3.scale.ordinal(),
        colorScale = d3.scale.linear();

    // Closure in charge of rendering the chart in the DOM.
    var chart = function(selection) {
        selection.each(function(data) {
            var dim;

            if (!data[0].length) {
                var dataLabels = extractDataLabels(data);
                xLabels = dataLabels.xLabels;
                yLabels = dataLabels.yLabels;
                data = normalize(dataLabels.data);
                dim = dimensions(data);
                data = flatten(data);
            } else {
                dim = dimensions(data);
                data = flatten(data);
                var min = d3.min(data);
                var max = d3.max(data);
                data = data.map(function(d) {
                   return (d - min) / (max - min);
                });
            }

            setScales(dim);
            colorScale.domain([0, 1])
                .range([minColor, maxColor]);

            var svg = d3.select(this).selectAll('svg').data([data]);

            var gEnter = svg.enter().append('svg').append('g');
            gEnter.append('g').attr('class', 'heatmap-tiles');
            gEnter.append('g').attr('class', 'x axis');
            gEnter.append('g').attr('class', 'y axis');

            // Update dimensions
            svg.attr('width', width)
                .attr('height', height);

            var g = svg.select('g')
                .attr('transform', 'translate(' + margin.left + ',' + margin.top + ')');

            var tilesContainer = g.select('.heatmap-tiles')
                .selectAll('rect')
                .data(data);

            var colorInterpolator = d3.interpolateRgb(minColor, maxColor);
            var tileWidth = chartWidth() / dim.x;
            var tileHeight = chartHeight() / dim.y;
            tilesContainer.enter().append('rect')
                .attr('x', function(_, index) { return (index % dim.x) * tileWidth + tileWidth / 2; })
                .attr('y', function(_, index) { return Math.floor(index / dim.x) * tileHeight + tileHeight / 2; })
                .attr('width', 0)
                .attr('height', 0)
                .style('fill', function(d) { return colorScale(d) });

            tilesContainer.exit().remove();

            tilesContainer.transition()
                .duration(transitionDuration)
                .attr('x', function(_, index) { return (index % dim.x) * tileWidth + 1; })
                .attr('y', function(_, index) { return Math.floor(index / dim.x) * tileHeight + 1; })
                .attr('width', tileWidth - 1)
                .attr('height', tileHeight - 1)
                .style('fill', function(d) { return colorScale(d) });

            setAxes(g.select('.x.axis'), g.select('.y.axis'), dim);
        });

        function setScales(dim) {
            if (xLabels !== null && xLabels.length === dim.x) {
                xScale.domain(xLabels);
            } else {
                xScale.domain(range(1, dim.x + 1));
            }
            xScale.rangeBands([0, chartWidth()]);

            if (yLabels !== null && yLabels.length === dim.y) {
                yScale.domain(yLabels);
            } else {
                yScale.domain(range(1, dim.y + 1));
            }

            yScale.rangeBands([0, chartHeight()]);
        }

        function setAxes(xAxisLabel, yAxisLabel, dim) {
            var xAxis = d3.svg.axis()
                .scale(xScale)
                .orient('top');

            var yAxis = d3.svg.axis()
                .scale(yScale)
                .orient('left');

            xAxisLabel.transition()
                .duration(transitionDuration)
                .call(xAxis);
            yAxisLabel.transition()
                .duration(transitionDuration)
                .call(yAxis);
        }

        function dimensions(data) {
            return { x: data[0].length, y: data.length };
        }

        function flatten(data) {
            return data.reduce(function(prev, curr) {
                Array.prototype.push.apply(prev, curr);
                return prev;
            });
        }
    };

    // Gets/sets the width of the chart.
    chart.width = function(val) {
        if (!arguments.length) return width;
        width = val;
        return this;
    };

    // Gets/sets the height of the chart.
    chart.height = function(val) {
        if (!arguments.length) return height;
        height = val;
        return this;
    };

    // Gets/sets the minimum color for the lowest value in the heatmap.
    chart.minColor = function(val) {
        if (!arguments.length) return minColor;
        minColor = d3.rgb(val);
        return this;
    };

    // Gets/sets the maximum color for the highest value in the heatmap.
    chart.maxColor = function(val) {
        if (!arguments.length) return maxColor;
        maxColor = d3.rgb(val);
        return this;
    };

    // Gets/sets the labels for the horizontal axis of this heatmap.
    chart.xLabels = function(val) {
        if (!arguments.length) return xLabels;
        xLabels = val;
        return this;
    };

    // Gets/sets the labels for the vertical axis of this heatmap.
    chart.yLabels = function(val) {
        if (!arguments.length) return yLabels;
        yLabels = val;
        return this;
    };

    // Gets/sets the transition duration for this chart's animations.
    chart.transitionDuration = function(val) {
        if (!arguments.length) return transitionDuration;
        transitionDuration = val;
        return this;
    };

    // Gets/sets the element from the data to extract as the y labels for the
    // vertical axis.
    chart.rowLabelName = function(val) {
        if (!arguments.length) return rowLabelName;
        rowLabelName = val;
        return this;
    };

    function extractDataLabels(data) {
        var dataLabels = {
            xLabels: Object.keys(data[0]).slice(1),
            yLabels: data.map(function(row) { return row.Name; }),
        };

        dataLabels.data = data.map(function(row) {
            return dataLabels.xLabels.map(function(key) {
                return row[key];
            });
        });

        return dataLabels;
    };

    function normalize(data) {
        var mins = data[0].map(function(_, index) {
            return d3.min(data, function(d) { return +d[index] });
        });
        var maxes = data[0].map(function(_, index) {
            return d3.max(data, function(d) { return +d[index] });
        });

        return data.map(function(row) {
            return row.map(function(elem, index) {
                return (elem - mins[index]) / (maxes[index] - mins[index]);
            });
        });
    }

    function chartWidth() {
        return width - margin.left - margin.right;
    }

    function chartHeight() {
        return height - margin.top - margin.bottom;
    }

    function range(start, stop, step) {
        if (typeof stop == 'undefined') {
            // one param defined
            stop = start;
            start = 0;
        }

        if (typeof step == 'undefined') {
            step = 1;
        }

        if ((step > 0 && start >= stop) || (step < 0 && start <= stop)) {
            return [];
        }

        var result = [];
        for (var i = start; step > 0 ? i < stop : i > stop; i += step) {
            result.push(i);
        }

        return result;
    }

    return chart;
}
