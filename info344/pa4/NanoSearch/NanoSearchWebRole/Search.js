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
            FetchSearchResults($('#query').val(), DisplaySearchResults)
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
        if (results.d == null) {
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
})();