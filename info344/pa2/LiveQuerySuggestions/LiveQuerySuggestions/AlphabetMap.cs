using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LiveQuerySuggestions
{
    /// <summary>
    /// Represents a map that has 27 keys. "a"-"z" and " ".
    /// </summary>
    /// <typeparam name="T">The type to hold as value in the map.</typeparam>
    public class AlphabetMap<T> : IEnumerable<KeyValuePair<char, T>>
    {
        private T[] elements;
        public int Count { get; set; }

        /// <summary>
        /// Gets the elment at c from the map.
        /// </summary>
        /// <param name="c">The key for the element.</param>
        /// <returns>The element at the key.</returns>
        public T this[char c]
        {
            get
            {
                return elements[LetterToIndex(c)];
            }
            set
            {
                int index = LetterToIndex(c);
                if (elements[index] == null && value != null)
                    ++Count;
                else if (elements[index] != null && value == null)
                    --Count;

                elements[index] = value;
            }
        }

        /// <summary>
        /// Creates a new AlphabetMap.
        /// </summary>
        public AlphabetMap()
        {
            elements = new T[27];
        }

        /// <summary>
        /// Checks if the map has the given child.
        /// </summary>
        /// <param name="c">The child to look for.</param>
        /// <returns>True if map contains child, false otherwise.</returns>
        public bool HasChild(char c)
        {
            return ValidKey(c) && elements[LetterToIndex(c)] != null;
        }

        public IEnumerator<KeyValuePair<char, T>> GetEnumerator()
        {
            return new AlphabetMapEnumerator<T>(elements);
        }

        private int LetterToIndex(char c)
        {
            c = char.ToLower(c);
            return c == ' ' ? 0 : c - 'a' + 1;
        }

        private bool ValidKey(char c)
        {
            c = char.ToLower(c);
            return c == ' ' || (c >= 'a' && c <= 'z');
        }

        /// <summary>
        /// An enumerator for AlphabetMap.
        /// </summary>
        public class AlphabetMapEnumerator<Q> : IEnumerator<KeyValuePair<char, Q>>
        {
            public int index;
            public Q[] elements;

            public AlphabetMapEnumerator(Q[] elements)
            {
                this.elements = elements;
                index = -1;
            }

            public KeyValuePair<char, Q> Current
            {
                get 
                {
                    int key = index == 0 ? ' ' : 'a' + index - 1;
                    return new KeyValuePair<char,Q>((char) key, elements[index]); 
                }
            }

            public bool MoveNext()
            {
                if (index < 0)
                    index = 0;
                else if (index < 27)
                    ++index;

                while (index < 27 && elements[index] == null)
                {
                    ++index;
                }

                return index < 27;
            }

            public void Reset()
            {
                index = -1;
            }

            public void Dispose() { }

            object System.Collections.IEnumerator.Current
            {
                get { throw new NotImplementedException(); }
            }
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            throw new NotImplementedException();
        }
    }
}
