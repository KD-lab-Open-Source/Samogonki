if exist lst0 del lst0 lst1
game.exe /3 /a hsWorld:%1 hsTrack:%2 second_verify disable_fp_exceptions time_to_restart:1800 repeate_times:4 dont_sleep
if exist lst0 fd lst0 lst1
:del lst_