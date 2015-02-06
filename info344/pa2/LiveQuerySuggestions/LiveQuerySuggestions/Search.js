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
        }).fail(function () {
            $('#suggestions').empty();
        });
    }

    function DisplaySuggestions(suggestions) {
        currentRequest = null;
        var $list = $('#suggestions').empty();
        if (suggestions === null)
            return;

        if (suggestions[0][0][0] === '<') {
            var $entry = $('<li class="list-group-item disabled">')
                .text('Could not find results for "' + 
                    $('#query').val() + '". Did you mean');
            $entry.append($badge);
            $list.append($entry);
        }

        for (var i = 0; i < suggestions.length; ++i) {
            var $entry = $('<li class="list-group-item">')
                .html(suggestions[i][0]);
            var $badge = $('<span class="badge">')
                .text(suggestions[i][1]);
            $entry.append($badge);
            $list.append($entry);
        }
    }
})();