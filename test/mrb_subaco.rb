##
## Subaco Test
##

assert("Subaco#hello") do
  t = Subaco.new "hello"
  assert_equal("hello", t.hello)
end

assert("Subaco#bye") do
  t = Subaco.new "hello"
  assert_equal("hello bye", t.bye)
end

assert("Subaco.hi") do
  assert_equal("hi!!", Subaco.hi)
end
