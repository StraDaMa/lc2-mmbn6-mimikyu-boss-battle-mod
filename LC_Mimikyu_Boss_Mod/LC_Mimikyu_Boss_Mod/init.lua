local exe6 = chaudloader.exedat.open("exe6.dat")
local exe6f = chaudloader.exedat.open("exe6f.dat")

chaudloader.util.edit_mpak(exe6, "exe6/data/msg/message_eng", function(mpak)
	chaudloader.util.merge_msgs_from_mod_directory(mpak, "msg_eng")
end)

chaudloader.util.edit_mpak(exe6, "exe6/data/msg/message_jap", function(mpak)
	chaudloader.util.merge_msgs_from_mod_directory(mpak, "msg_jap")
end)

chaudloader.util.edit_mpak(exe6, "exe6/data/msg/message_cht", function(mpak)
	chaudloader.util.merge_msgs_from_mod_directory(mpak, "msg_cht")
end)

chaudloader.util.edit_mpak(exe6, "exe6/data/msg/message_chs", function(mpak)
	chaudloader.util.merge_msgs_from_mod_directory(mpak, "msg_chs")
end)

chaudloader.util.edit_mpak(exe6f, "exe6f/data/msg/message_eng", function(mpak)
	chaudloader.util.merge_msgs_from_mod_directory(mpak, "msg_eng_f")
end)

chaudloader.util.edit_mpak(exe6f, "exe6f/data/msg/message_jap", function(mpak)
	chaudloader.util.merge_msgs_from_mod_directory(mpak, "msg_jap_f")
end)

chaudloader.util.edit_mpak(exe6f, "exe6f/data/msg/message_cht", function(mpak)
	chaudloader.util.merge_msgs_from_mod_directory(mpak, "msg_cht_f")
end)

chaudloader.util.edit_mpak(exe6f, "exe6f/data/msg/message_chs", function(mpak)
	chaudloader.util.merge_msgs_from_mod_directory(mpak, "msg_chs_f")
end)

require("LC_Mimikyu_Boss_Mod")