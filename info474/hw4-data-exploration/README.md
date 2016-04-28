# Homework 4 - Data Exploration

I chose to implement a basic time series chart for this assignment, since they
are some of the most commonly used charts in monitoring applications that I've
used in the industry. I thought it would be a good idea to understand how to
implement them since I used them so often. The chart has two axes: an x-axis
representing time and a y-axis representing counts. I'm using a dataset provided
by the city of Seattle that shows the pedestrian and biker count on the
Burke-Gilman trail from Jan 1, 2014 through 2016. The data is broken down
further by pedestrians and bikers going North/South.

A time chart is the optimal way to display this data since it is inherently
temporal. The City of Seattle decided to capture values every hour, so having
that level of granularity of time is also best expressed using a time series.
Additionally, I wanted to be able to compare pedestrian/biker frequencies over
time, so plotting different points on the same time series allows for comparing
these values rather easily. I found it rather useful to place data this way.
You could differentiate the different values by color and compare their position
to perceive differences.

I added 2 controls to this chart, both related to time. One control is for the
start date of the data, and the second one is for the time range (from 1 to 7
days). Time series are meant to help analyze data over time, so it is essential
to be able to control what time period you're displaying and analyze trends
over time. It is also essential to have bigger/smaller time spans based on the
need for analysis. Sometimes I want to see trends for a single day, and
and sometimes I may care about trends for an entire week.
