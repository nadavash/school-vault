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
                return elements[c == ' ' ? 26 : c - 'a'];
            }
            set
            {
                int index = c == ' ' ? 26 : c - 'a';
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

        public IEnumerator<T> GetEnumerator()
        {
            return new AlphabetMapEnumerator<T>(elements);
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
