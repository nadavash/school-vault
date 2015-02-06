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
        if (suggestions === null)
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
})();