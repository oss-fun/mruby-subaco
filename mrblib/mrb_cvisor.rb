class CVisor
    def allow_ipaddr(ipaddr)
	    set_whitelist(ipaddr.split(".").map!(&:to_i))
    end
end

