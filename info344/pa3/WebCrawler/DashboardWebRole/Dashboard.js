(function () {
    $(document).ready(function () {
        console.log('hello');
        $('#play').click(Crawl);
        $('#pause').click(Pause);
        $('#clear-urls').click(ClearUrls);
    });

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

    function MakeAjaxCall(call, data, onsuccess) {
        $.ajax({
            type: 'get',
            url: 'AdminWebService.asmx/' + call,
            data: data,
            contentType: 'application/json; charset=utf-8',
            dataType: 'json',
            success: onsuccess
        }).fail(function (jxhr, msg) {
            toastr.error(msg);
        });
    }
})();