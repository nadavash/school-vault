using Microsoft.VisualBasic.CompilerServices;
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
    public class RobotsParser
    {
        public const string USER_AGENT = "user-agent";
        public const string SITEMAP = "sitemap";
        public const string DISSALLOW = "disallow";

        public string UserAgent { get; private set; }
        public List<string> SiteMaps { get; private set; }
        public List<string> Disallowed { get; private set; }

        public RobotsParser(string userAgent)
        {
            UserAgent = userAgent;
            SiteMaps = new List<string>();
            Disallowed = new List<string>();
        }

        public void Parse(StreamReader reader)
        {
            string line;
            string[] parts;
            bool matchingAgent = true;

            while ((line = reader.ReadLine()) != null)
            {
                line = line.Trim();
                if (line[0] == '#')
                    continue;

                parts = line.Split(new char[] {':'}, 2);
                if (parts.Length < 2)
                {
                    Trace.TraceError("Failed to parse robots.txt line: " + line);
                    continue;
                }
                parts[0] = parts[0].ToLower();
                parts[1] = parts[1].Trim();

                // Check if a user agent is specified.
                if (parts[0] == USER_AGENT)
                {
                    matchingAgent = Operators.LikeString(UserAgent, parts[1],
                        Microsoft.VisualBasic.CompareMethod.Text);
                }
                else if (matchingAgent)
                    ParseLine(parts);
            }
        }

        public bool IsUrlAllowed(string url)
        {
            Uri uri = new Uri(url);
            string path = "/" + uri.GetComponents(UriComponents.Path, UriFormat.Unescaped);
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
                case SITEMAP:
                    SiteMaps.Add(parts[1]);
                    break;
                case DISSALLOW:
                    Disallowed.Add(parts[1]);
                    break;
                default:
                    Trace.TraceError(
                        string.Format("Unrecognized robots.txt attribute while parsing: '{0}'",
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
