package main

// SuggestService defines service methods for search query completion.
type SuggestService struct {
}

// Hello returns a 'hello' message.
func (s *SuggestService) Hello(arg *struct{}, reply *string) error {
    *reply = "Hello, world!"
    return nil
}
