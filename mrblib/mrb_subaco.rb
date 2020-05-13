class Subaco
    def allow_ipaddr(ipaddr)
	    set_whitelist(ipaddr.split(".").map!(&:to_i))
    end
end
module Util
    def pid_inum(pid)
       File.stat("/proc/" + pid.to_s + "/ns/pid").ino
    end
    module_function :pid_inum
end

