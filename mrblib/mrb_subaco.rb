class Subaco
  def allow_global_network
    set_global_network 1
  end
  def deny_global_network
    set_global_network 0
  end
  def label=(label)
    @label = label.unpack("H*")[0].hex
    set_label @label
  end
  def label
    @label
  end
end

module Util
  def pid_inum(pid)
    File.stat("/proc/" + pid.to_s + "/ns/pid").ino
  end
  module_function :pid_inum
end

