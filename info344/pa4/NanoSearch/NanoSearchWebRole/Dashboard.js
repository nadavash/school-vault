(function () {
    var UPDATE_TIME_MILLIS = 10000;

    $(document).ready(function () {
        console.log('Initializing...');

        Update();
        // Set update interval
        setInterval(Update, UPDATE_TIME_MILLIS);


        $('#play').click(Crawl);
        $('#pause').click(Pause);
        $('#clear-urls').click(ClearUrls);
        $('#clear-index').click(DeleteIndex);
        $('#clear-errors').click(DeleteErrors);
        $('#refresh').click(Update);

        $('#search').click(Search);
    });

    function Update() {
        MakeAjaxCall('GetDashboardData', {}, function (results) {
            console.log(results.d);
            
            $('.crawler-table').remove();
            var crawlers = results.d.CrawlerStates;
            var totalCrawled = 0;
            for (var i = 0; i < crawlers.length; ++i) {
                totalCrawled += crawlers[i].UrlsIndexed;
                CreateCrawlerTable(crawlers[i]);
            }

            $('#metric-indexed').text(totalCrawled);
            $('#metric-queued').text(results.d.NumQueuedUrls);
        });
        GetErrors();
        GetTrieData();
    }

    function Crawl() {
        console.log('Crawl command.');
        MakeAjaxCall('StartCrawler', {}, function (results) {
            toastr.info(results.d);
        });
    }

    function Pause() {
        console.log('Pause command.');
        MakeAjaxCall('StopCrawler', {}, function (results) {
            toastr.info(results.d);
        });
    }

    function ClearUrls() {
        console.log('Clear urls.');
        MakeAjaxCall('ClearUrlQueue', {}, function (results) {
            toastr.info(results.d);
        });
    }

    function DeleteIndex() {
        console.log('Delete index.');
        MakeAjaxCall('DeleteIndex', {}, function (results) {
            toastr.info(results.d);
        });
    }

    function DeleteErrors() {
        console.log('Delete errors.');
        MakeAjaxCall('DeleteErrors', {}, function (results) {
            toastr.info(results.d);
        });
    }

    function GetErrors() {
        console.log('Get errors.');
        MakeAjaxCall('GetErrors', {}, function (results) {
            var list = $('#errors').empty();
            var errs = results.d;
            if (!errs) return;
            console.log(results.d)
            for (var i = 0; i < errs.length; ++i) {
                var msg = errs[i];
                var text = msg.Message + ' ' + msg.Status + ' ' + msg.Url;
                list.append(
                    $('<li class="list-group-item">').text(text));
            }
        });
    }

    function GetTrieData() {
        console.log('Get trie data.');
        $.ajax({
            type: 'get',
            url: 'QuerySuggestionsService.asmx/NumUrls',
            contentType: 'application/json; charset=utf-8',
            dataType: 'json',
            success: function (results) {
                $('#metric-trie').text(results.d);
            }
        }).fail(function (jxhr, msg, errorThrown) {
            toastr.error(msg + ': ' + errorThrown);
        });

        $.ajax({
            type: 'get',
            url: 'QuerySuggestionsService.asmx/LastTitle',
            contentType: 'application/json; charset=utf-8',
            dataType: 'json',
            success: function (results) {
                var title = results.d === null ? '' : results.d;
                $('#metric-title').text(title);
            }
        }).fail(function (jxhr, msg, errorThrown) {
            toastr.error(msg + ': ' + errorThrown);
        });
    }

    function Search() {
        console.log('Searching. ' + $('#query').val());
        PostAjaxCall('GetTitleFromIndex', {
            url: $('#query').val()
        }, function (response) {
            var text = 'Could not find url in index.';
            if (response.d != null) {
                text = response.d;
            }
            $('#results').html(text);
        });
    }

    function MakeAjaxCall(call, data, onsuccess) {
        $.ajax({
            type: 'get',
            url: 'AdminWebService.asmx/' + call,
            data: data,
            contentType: 'application/json; charset=utf-8',
            dataType: 'json',
            success: onsuccess
        }).fail(function (jxhr, msg, errorThrown) {
            toastr.error(msg + ': ' + errorThrown);
        });
    }

    function PostAjaxCall(call, data, onsuccess) {
        $.ajax({
            type: 'post',
            url: 'AdminWebService.asmx/' + call,
            data: JSON.stringify(data),
            contentType: 'application/json; charset=utf-8',
            dataType: 'json',
            success: onsuccess
        }).fail(function (jxhr, msg, errorThrown) {
            toastr.error(msg + ': ' + errorThrown);
        });
    }

    function CreateCrawlerTable(crawlerState) {
        var $table = $('#default-state').clone()
            .attr('id', crawlerState.RowKey)
            .addClass('crawler-table')
            .toggle();
        $table.find('.state-heading').text(crawlerState.RowKey)
            .append(': ' + StatusText(crawlerState.Status));
        $table.find('.metric-crawled').text(crawlerState.UrlsCrawled);
        $table.find('.metric-indexed').text(crawlerState.UrlsIndexed);
        $table.find('.metric-errors').text(crawlerState.ErrorCount);
        $table.find('.metric-cpu').text(crawlerState.CpuUtilization + '%');
        $table.find('.metric-ram-used').text(crawlerState.RamUsed + "MB");
        $table.find('.metric-ram-available').text(crawlerState.RamAvailable + "MB");

        if (crawlerState.LastTenUrls !== null &&
            crawlerState.LastTenUrls !== '') {
            var lastTen = crawlerState.LastTenUrls.split(',');
            var $list = $table.find('.list-group');
            for (var i = 0; i < lastTen.length; ++i) {
                $list.append($('<li class="list-group-item">')
                    .text(lastTen[i]));
            }
        }

        $('#dashboard').append($table);
    }

    function StatusText(status) {
        switch (status) {
            case 0:
                return 'paused';
            case 1:
                return 'idle';
            case 2:
                return 'loading';
            case 3:
                return 'running';
            default:
                return 'unknown';
        }
    }
})();