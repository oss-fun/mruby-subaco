MRuby::Gem::Specification.new('mruby-subaco') do |spec|
  
  spec.license = 'MIT'
  spec.authors = 'Yuki Nakata'
  spec.add_dependency('mruby-file-stat', :github => 'ksss/mruby-file-stat')
  spec.add_dependency('mruby-pack', :github => 'iij/mruby-pack')
end
