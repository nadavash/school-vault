import urllib
import urllib2

url = "http://nanosearch.cloudapp.net/QuerySuggestionsService.asmx/GetSuggestionsRanked?%s"
prefix = "test"
num = 5

values = {
	'prefix': prefix,
	'max': num
}

data = urllib.urlencode(values)
req = urllib2.Request(url, data)
response = urllib2.urlopen(req)
the_page = response.read()