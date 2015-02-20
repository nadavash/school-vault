using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace CrawlerLibrary
{
    class RobotsParser
    {
        public string UserAgent { get; private set; }
        public List<string> SiteMaps { get; private set; }
        public List<string> Disallowed { get; private set; }

        public RobotsParser(string userAgent)
        {
            UserAgent = userAgent;
            SiteMaps = new List<string>();
            Disallowed = new List<string>();
        }

        public bool Parse(StreamReader reader)
        {
            string line;
            string[] parts;
            bool matchingAgent = true;

            while ((line = reader.ReadLine()) != null)
            {
                line = line.Trim();
                if (line[0] == '#')
                    continue;

                parts = line.Split(':');
                if (parts.Length != 2)
                {
                    Clear();
                    return false;
                }

                parts[1] = parts[1].Trim();

                // Check if a user agent is specified.
                if (parts[0] == "User-agent")
                {
                    Regex regex = new Regex(parts[1]);
                    matchingAgent = regex.IsMatch(UserAgent);
                }
                else if (matchingAgent)
                    ParseLine(parts);
            }

            return true;
        }

        public bool IsUrlAllowed(string url)
        {
            Uri uri = new Uri(url);
            string path = uri.GetComponents(UriComponents.Path, UriFormat.Unescaped);
            foreach (var s in Disallowed)
            {
                if (path.StartsWith(s))
                    return false;
            }

            return true;
        }

        private void ParseLine(string[] parts)
        {
            switch (parts[0])
            {
                case "Sitemap":
                    SiteMaps.Add(parts[1]);
                    break;
                case "Disallow":
                    Disallowed.Add(parts[1]);
                    break;
                default:
                    Trace.TraceError(
                        string.Format("Unrecognized robots.txt attribute while parsing: '{}'",
                                      parts[0]));
                    break;
            }
        }

        public void Clear()
        {
            SiteMaps.Clear();
            Disallowed.Clear();
        }
    }
}
