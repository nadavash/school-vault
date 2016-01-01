package trie_test

import (
    "github.com/nadava/school-vault/info344/suggest/trie"
    "testing"
)

func TestConstructor(t *testing.T) {
    am := trie.NewAlphabetMap()
    if am == nil {
        t.Error("NewAlphabetMap constructor returned nil")
    }
}

func TestGetAndPut(t *testing.T) {
    am := trie.NewAlphabetMap()
    am.Put('a', "hi")

    str, ok := am.Get('a')
    if str != "hi" {
        t.Errorf("Expected \"hi\"; got %q instead", str)
    }

    if !ok {
        t.Errorf("Get method returned false for existing key '%x'", str)
    }
}

func TestForEach(t *testing.T) {
    am := trie.NewAlphabetMap()
    am.Put('a', 1)
    am.Put('b', 2)
    am.Put('c', 3)
    am.Put('d', 4)

    count := 0
    am.ForEach(func (key byte, value interface{}) bool {
        count++
        if count > 4 {
            t.Error("ForEach should not iterate on more than 4 items")
        }
        if int(key - 'a' + 1) != value.(int) {
            t.Errorf("Expected %d; got %d", value, key - 'a' + 1)
        }
        return true
    })

    if count != 4 {
        t.Errorf("ForEach only iterated %d times. Should iterate 4 times", count)
    }
}

func TestForEachStop(t *testing.T) {
    am := trie.NewAlphabetMap()

    chars := []byte{'a', 'b', 'c', 'd'}
    for index, char := range chars {
        am.Put(char, &chars[index])
    }

    count := 0
    am.ForEach(func (key byte, value interface{}) bool {
        count++
        if key != *value.(*byte) {
            t.Errorf("%x != %x", key, *value.(*byte))
        }
        return count < 2
    })

    if count != 2 {
        t.Errorf("ForEach should terimnate after 2 loops. Instead terminated after %d", count)
    }
}
