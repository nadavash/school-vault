(function () {
    var MAX_RESULTS = 10;
    var TIMEOUT = 200;

    var currentRequest = null;
    var currentSearch = null;
    var instantTimeout = null;

    $(document).ready(function () {
        $('#query').focus()
            .on('input propertychange paste', function () {
                FetchSuggestions($(this).val(), MAX_RESULTS, function (results) {
                    DisplaySuggestions(results.d);
                });
                
                if (instantTimeout !== null) {
                    clearTimeout(instantTimeout);
                }
                instantTimeout = setTimeout(function () {
                    instantTimeout = null;
                    DoQuery(true);
                }, TIMEOUT);
            });
        $('#search-button').click(function () {
            DoQuery();
        });
        $('#query').keypress(function (e) {
            if ((e.keyCode | e.which) === 13) {
                DoQuery();
            }
        });
    });

    function DoQuery(instant) {
        if (currentRequest !== null) {
            currentRequest.abort();
            currentRequest = null;
        }
        var query = $('#query').val();
        if (query.trim() === '') {
            return;
        }
        $('.loading').show();
        if (!instant) { $('#suggestions').empty(); }
        $('#players').empty();
        $('#results-list').empty();
        FetchPlayerData(query, DisplayPlayerData);
        FetchSearchResults(query, DisplaySearchResults);
    }

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
        if (currentSearch !== null) {
            currentSearch.abort();
            currentSearch = null;
        }

        currentSearch = $.ajax({
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
        $('#link-load').hide();
        var $resultList = $('#results-list').empty();
        if (results.d === null || results.d.length === 0) {
            $resultList.append($('<p>')
                    .append('Could not find search results for <strong>'
                        + $('#query').val() + '</strong>'));
            return;
        }

        var searchResults = results.d;
        for (var i = 0; i < searchResults.length; ++i) {
            var $listResult = $('<li class="list-group-item">')
                .append($('<span class="badge">')
                    .text(searchResults[i].Rank))
                .append($('<a class="title-link list-group-item-heading">')
                    .html(searchResults[i].Title)
                    .attr('href', searchResults[i].Url))
                .append($('<p class="link-url list-group-item-text">')
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
        $('#player-load').hide();
        var $container = $('#players').empty();
        if (results.players === null || results.players.length === 0) {
            return;
        }

        var players = results.players;
        $container.append(BuildPlayerCard(players[0]));
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
                                    <td>' + ValueOrDash(player.gamesPlayed) + '</td> \
                                    <td>' + ValueOrDash(player.fieldGoalPercentage) + '</td> \
                                    <td>' + ValueOrDash(player.threePointPercentage) + '</td> \
                                    <td>' + ValueOrDash(player.freeThrowPercentage) + '</td> \
                                    <td>' + ValueOrDash(player.pointsPerGame) + '</td> \
                                </tr> \
                            </tbody> \
                        </table> \
                    </div> \
                </div>');
    }

    function ValueOrDash(val) {
        return val !== null && val != 0 ? val : '--';
    }
})();