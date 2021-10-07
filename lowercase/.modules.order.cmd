cmd_lowercase/modules.order := {  :; } | awk '!x[$$0]++' - > lowercase/modules.order
