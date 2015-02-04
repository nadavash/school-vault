using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LiveQuerySuggestions
{
    public class AlphabetMap<T> : IEnumerable<T>
    {
        private T[] elements;
        public int Count { get; set; }

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

        public AlphabetMap()
        {
            elements = new T[27];
        }

        public bool HasChild(char c)
        {
            return ValidKey(c) && elements[LetterToIndex(c)] != null;
        }

        public IEnumerator<T> GetEnumerator()
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

        public class AlphabetMapEnumerator<Q> : IEnumerator<Q>
        {
            public int index;
            public Q[] elements;

            public AlphabetMapEnumerator(Q[] elements)
            {
                this.elements = elements;
                index = -1;
            }

            public Q Current
            {
                get { return elements[index]; }
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
