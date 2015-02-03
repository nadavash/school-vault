(function () {
    $(document).ready(function () {
        $('#query').focus()
            .on('input propertychange paste', function () {
                FetchSuggestions($(this).val(), 5, function (results) {
                    DisplaySuggestions(results.d);
                });
            });
    });

    function FetchSuggestions(prefix, max, onsuccess) {
        $.ajax({
            type: 'get',
            url: 'QuerySuggestions.asmx/GetSuggestions',
            data: {
                prefix: JSON.stringify(prefix),
                max: max
            },
            contentType: "application/json; charset=utf-8",
            dataType: 'json',
            success: onsuccess
        });
    }

    function DisplaySuggestions(suggestions) {
        var $list = $('#suggestions').empty();
        for (var i = 0; i < suggestions.length; ++i) {
            var $entry = $('<li>');
            $entry.text(suggestions[i]);
            console.log(suggestions[i]);
            $list.append($entry);
        }
    }
})();