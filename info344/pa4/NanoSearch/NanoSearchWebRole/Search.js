(function () {
    var MAX_RESULTS = 10;

    var currentRequest = null;
    $(document).ready(function () {
        $('#query').focus()
            .on('input propertychange paste', function () {
                FetchSuggestions($(this).val(), MAX_RESULTS, function (results) {
                    DisplaySuggestions(results.d);
                });
            });
        $('#search-button').click(function () {
            var query = $('#query').val();
            FetchPlayerData(query, DisplayPlayerData);
            FetchSearchResults(query, DisplaySearchResults);
        });
    });

    function FetchSuggestions(prefix, max, onsuccess) {
        if (currentRequest !== null) {
            currentRequest.abort();
        }
        currentRequest = $.ajax({
            type: 'get',
            url: 'QuerySuggestionsService.asmx/GetSuggestionsRanked',
            data: {
                prefix: JSON.stringify(prefix),
                max: max
            },
            contentType: 'application/json; charset=utf-8',
            dataType: 'json',
            success: onsuccess
        });
    }

    function DisplaySuggestions(suggestions) {
        currentRequest = null;
        var $list = $('#suggestions').empty();
        if (suggestions === null || suggestions.length == 0)
            return;

        var allFuzzy = true;
        for (var i = 0; i < suggestions.length; ++i) {
            if (suggestions[i][0][0] != '<') {
                allFuzzy = false;
            }
            var $entry = $('<li class="list-group-item">')
                .html(suggestions[i][0]);
            var $badge = $('<span class="badge">')
                .text(suggestions[i][1]);
            $entry.append($badge);
            $list.append($entry);
        }

        if (allFuzzy) {
            var $entry = $('<li class="list-group-item disabled">')
                .text('Could not find matches for "' +
                    $('#query').val() + '". How about...');
            $list.prepend($entry);
        }
    }

    function FetchSearchResults(query, onsuccess) {
        $.ajax({
            type: 'get',
            url: 'SearchService.asmx/GetSearchResults',
            data: {
                q: JSON.stringify(query),
            },
            contentType: 'application/json; charset=utf-8',
            dataType: 'json',
            success: onsuccess
        });
    }

    function DisplaySearchResults(results) {
        if (results.d === null) {
            return;
        }

        var $resultList = $('#results-list').empty();

        var searchResults = results.d;
        for (var i = 0; i < searchResults.length; ++i) {
            var $listResult = $('<li class="list-group-item">')
                .append($('<a class="list-group-item-heading">')
                    .html(searchResults[i].Title)
                    .attr('href', searchResults[i].Url))
                .append($('<p class="list-group-item-text">')
                    .text(searchResults[i].Url));
            $resultList.append($listResult);
        }
    }

    function FetchPlayerData(query, onsuccess) {
        $.ajax({
            type: 'get',
            crossDomain: true,
            url: 'http://ec2-54-149-235-53.us-west-2.compute.amazonaws.com/getPlayerData.php',
            dataType: 'jsonp',
            jsonpCallback: 'callback',
            data: {
                q: query,
            },
            contentType: 'application/json; charset=utf-8',
            success: onsuccess
        });
    }

    function DisplayPlayerData(results) {
        if (results.players === null) {
            return;
        }
        console.log(results);
        var $container = $('#players').empty();
        var players = results.players;
        for (var i = 0; i < players.length; ++i) {
            $container.append(BuildPlayerCard(players[i]));
        }
    }

    function BuildPlayerCard(player) {
        var imgUrl = 'http://i.cdn.turner.com/nba/nba/.element/img/2.0/sect/statscube/players/large/' 
            + player.playerName.toLowerCase().replace(' ', '_') + '.png';
        return $('<div class="player-card panel panel-default">')
            .html('<img class="player-img" src="' + imgUrl + '" alt="player profile image" \
                onerror="if (this.src != \'error.jpg\') this.src = \'/assets/stock.png\';" /> \
                    <div class="player-info"> \
                        <h3>' + player.playerName + '</h3> \
                        <table class="player-stats table"> \
                            <caption>Stats</caption> \
                            <thead> \
                                <tr class="player-stats-row"> \
                                    <th>GP</th> \
                                    <th>FGP</th> \
                                    <th>TPP</th> \
                                    <th>FTP</th> \
                                    <th>PPG</th> \
                                </tr> \
                            </thead> \
                            <tbody> \
                                <tr> \
                                    <td>' + player.gamesPlayed + '</td> \
                                    <td>' + player.fieldGoalPercentage + '</td> \
                                    <td>' + player.threePointPercentage + '</td> \
                                    <td>' + player.freeThrowPercentage + '</td> \
                                    <td>' + player.pointsPerGame + '</td> \
                                </tr> \
                            </tbody> \
                        </table> \
                    </div> \
                </div>');
    }
})();