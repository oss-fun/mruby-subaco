class Subaco
    def allow_global_network(macaddr)
        set_global_network(macaddr, 1)
    end
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

