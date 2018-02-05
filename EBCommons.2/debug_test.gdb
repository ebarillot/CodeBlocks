directory /userdev/ebarillot/projets/bfrance/c/test_unit
set breakpoint pending on
set print array on
set print symbol on
set print array-indexes on
set print elements 500
set print null-stop
set print pretty on
#winheight SRC -8
#winheight CMD +8

break main
# break test
# break begin
# break run
break test_Erreur
# break test_LRUBuf
# break test_LRUBuf_dyn

run
