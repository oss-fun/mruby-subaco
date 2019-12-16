##
## CVisor Test
##

assert("CVisor#hello") do
  t = CVisor.new "hello"
  assert_equal("hello", t.hello)
end

assert("CVisor#bye") do
  t = CVisor.new "hello"
  assert_equal("hello bye", t.bye)
end

assert("CVisor.hi") do
  assert_equal("hi!!", CVisor.hi)
end
