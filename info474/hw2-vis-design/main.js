/// <reference path="./typings/jquery/jquery.d.ts" />
(function() {
    'use strict';

    $(document).ready(function() {
        loadBacteriumData()
            .then(csvToJson)
            .then(function(data) {
                plotEffectiveness(data);
                plotGramStaining(data);
                plotCorrelation(data);
            })
    });

    function plotEffectiveness(data) {
        var barData = [
            {
                name: 'Penicilin',
                x: data['Bacteria'],
                y: data['Penicilin'],
                type: 'bar'
            },
            {
                name: 'Streptomycin',
                x: data['Bacteria'],
                y: data['Streptomycin'],
                type: 'bar'
            },
            {
                name: 'Neomycin',
                x: data['Bacteria'],
                y: data['Neomycin'],
                type: 'bar'
            }
        ];

        var layout = {
            title: 'Antibiotics effectiveness on bacteria',
            yaxis: {
                title: 'Minimum Inhibitory Concentration (MIC)'
            }
        };

        Plotly.newPlot('vis1-bar-chart', barData, layout, {staticPlot: true});
    }

    function plotGramStaining(data) {
        var posStain = {
            Streptomycin: [],
            Penicilin: [],
            Neomycin: []
        };
        var negStain = {
            Streptomycin: [],
            Penicilin: [],
            Neomycin: []
        };

        for (var i = 0; i < data['Bacteria'].length; ++i) {
            if (data['Gram_Staining'][i]) {
                posStain['Streptomycin'].push(data['Streptomycin'][i]);
                posStain['Penicilin'].push(data['Penicilin'][i]);
                posStain['Neomycin'].push(data['Neomycin'][i]);
            } else {
                negStain['Streptomycin'].push(data['Streptomycin'][i]);
                negStain['Penicilin'].push(data['Penicilin'][i]);
                negStain['Neomycin'].push(data['Neomycin'][i]);
            }
        }

        // Average!
        ['Neomycin', 'Penicilin', 'Streptomycin'].forEach(function(key) {
           posStain[key] = posStain[key].reduce(function(prev, current) {
               return (prev + current) / 2;
           });
           negStain[key] = negStain[key].reduce(function(prev, current) {
               return (prev + current) / 2;
           }) ;
        });

        var keys = ['Negative gram staining', 'Positive gram staining'];
        var stainingData = [
            {
                name: 'Penicilin',
                x: keys,
                y: [negStain['Penicilin'], posStain['Penicilin']],
                type: 'bar'
            },
            {
                name: 'Neomycin',
                x: keys,
                y: [negStain['Neomycin'], posStain['Neomycin']],
                type: 'bar'
            },
            {
                name: 'Streptomycin',
                x: keys,
                y: [negStain['Streptomycin'], posStain['Streptomycin']],
                type: 'bar'
            },
        ];

        var layout = {
            title: 'Antibiotics effectiveness on negative vs. positive gram ' +
                   'staining bacteria',
            yaxis: {
                title: 'Average Minimum Inhibitory Concentration (MIC)'
            }
        };

        Plotly.newPlot('vis2-box-plot', stainingData, layout,
                       {staticPlot: true});
    }

    function plotCorrelation(data) {
        var correlationData = [
            {
                x: data['Penicilin'],
                y: data['Neomycin'],
                mode: 'markers',
                type: 'scatter'
            }
        ];

        var layout = {
            title: 'Penicilin vs Neomycin for Bacetria Treatment',
            xaxis: {
                title: 'Penicilin MIC'
            },
            yaxis: {
                title: 'Neomycin MIC'
            }
        };

        Plotly.newPlot('vis3-scatter1', correlationData, layout,
                       {staticPlot: true});

        var correlationData = [
            {
                x: data['Penicilin'],
                y: data['Streptomycin'],
                mode: 'markers',
                type: 'scatter'
            }
        ];

        var layout2 =  {
            title: 'Penicilin vs Streptomycin for Bacetria Treatment',
            xaxis: {
                title: 'Penicilin MIC'
            },
            yaxis: {
                title: 'Streptomycin MIC'
            }
        };

        Plotly.newPlot('vis3-scatter2', correlationData, layout2,
                       {staticPlot: true});

        var correlationData = [
            {
                x: data['Neomycin'],
                y: data['Streptomycin'],
                mode: 'markers',
                type: 'scatter'
            }
        ];

        var layout3 =  {
            title: 'Neomycin vs Streptomycin for Bacetria Treatment',
            xaxis: {
                title: 'Neomycin MIC'
            },
            yaxis: {
                title: 'Streptomycin MIC'
            }
        };

        Plotly.newPlot('vis3-scatter3', correlationData, layout3,
                       {staticPlot: true});
    }

    // Returns a promise to a ajax GET request to fetch the antibiotics csv
    // file from the server.
    function loadBacteriumData() {
        return $.get('./antibiotics_data.csv', null, null, 'text');
    }

    // Converts the given csv antibiotics data and converts to a json object
    // containing arrays with the given values for each feature/field in the
    // csv.
    function csvToJson(csv) {
        var data = csv.split('\n')
            .map(function(line) {
                return line.trim().split(',');
            });
        var keys = data[0];
        var data = data.slice(1);

        var json = {};
        keys.forEach(function(key, index) {
           json[key] = data.map(function(row) {
               return row[index];
           });
        });

        json['Gram_Staining'] = json['Gram_Staining'].map(function(val) {
            return val === 'positive';
        });
        ['Neomycin', 'Penicilin', 'Streptomycin'].forEach(function(key) {
            json[key] = json[key].map(function(val) {
                return Number(val);
            });
        });

        return json;
    }

})();