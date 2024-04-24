idf_component_register(
	SRCS
		"main.c" 
		"custom_mbcrc.c" 
		"csv_sd.c"
		"all_variables.c" 
		"sd_card.c"
		"rs485_custom.c" 
		"heartbeat.c"
		
	INCLUDE_DIRS 
		"."
	
	EMBED_TXTFILES web/index.html
	EMBED_TXTFILES eita.jpg
)
