# mruby-subaco   [![Build Status](https://travis-ci.org/chikuwait/mruby-subaco.svg?branch=master)](https://travis-ci.org/chikuwait/mruby-subaco)
Subaco class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'chikuwait/mruby-subaco'
end
```
## example
```ruby
p Subaco.hi
#=> "hi!!"
t = Subaco.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the MIT License:
- see LICENSE file
