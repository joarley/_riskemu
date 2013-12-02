gateway = {
    logfile = "./log/LoginServer_" .. os.date("%Y%m%d%H%M%S") .. ".log",
    patch = {
	    version = 1753,
	    address = "localhost/patch"
    },

    launcher = {
        listen_address = {
            address = "localhost",
		    port = 10101
		},
    },
    
    authserver = {
        listen_address = {
            address = "localhost",
		    port = 1808,
		    max_Connections = 10
		}
    }
}
