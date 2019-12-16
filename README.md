# mruby-cvisor   [![Build Status](https://travis-ci.org/chikuwait/mruby-cvisor.svg?branch=master)](https://travis-ci.org/chikuwait/mruby-cvisor)
CVisor class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'chikuwait/mruby-cvisor'
end
```
## example
```ruby
p CVisor.hi
#=> "hi!!"
t = CVisor.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the MIT License:
- see LICENSE file
