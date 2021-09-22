class Subaco
  def initialize(name, nic, mac_addr, dev_no, func_no)
    @name = name
    @nic = nic
    @mac_addr = mac_addr
    @dev_no = dev_no
    @func_no = func_no
    @ip_addr = nil

    connect_with_vmm(@mac_addr, @dev_no, @func_no)
  end
  def assign_ip_addr
    addr = []
    4.times do |n|
      addr << get_ip_addr(@dev_no, @func_no, n).to_s
    end
    @ip_addr = addr.join('.') << "/24"

    system("ip link set #{@nic} netns #{@name}")
    system("ip netns exec #{@name} ip addr add #{@ip_addr} dev #{@nic}")
    system("ip netns exec #{@name} ip link set #{@nic} up")
  end
  def ip_addr
    @ip_addr
  end
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

