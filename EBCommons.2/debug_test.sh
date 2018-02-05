#!/usr/bin/ksh

####!/bin/bash

# Script de lancement des test du programme de chargement
# des informations négatives
# pour un remettant donné.
# Author - Emmanuel Barillot
# Usage: see usage() function, below

script_name="debug_test.sh"


usage(){
  print $script_name - debug du programme de chargement des informations négatives
  print "Usage: $script_name [-r] [-t <code>] [-p <plf>] <Module à tester>"
  print "  -r         mode rollback"
  print "  -p <plf>   code plateforme: IA, UA (DEV par defaut)"
  print "  -h         help"
}

logPath="${HOME}/projets/$(basename $PROJET)/log"
dataPath="${HOME}/projets/$(basename $PROJET)/data"
#progPath=$EXEC
progPath=.
prog_prfx=test_


while getopts ":rp:h" arg
do
  case $arg in
      r)
          mode_rollback="oui"
      ;;
      p)
          plf=$OPTARG
      ;;
      :)
          echo " l'option -$OPTARG a besoin d'un argument " ; exit
      ;;
      h|*)
          usage; exit 1
      ;;
      \?)
          usage; exit 1
      ;;
  esac
done

plf_defaut="DEV"
plateforme=${plf:=${plf_defaut}}

case $plateforme in
    "IA")
        export OUTIL=$PROJET/outils_IA
    ;;
    "UA")
        export OUTIL=$PROJET/outils_UA
    ;;
    "DEV")
    ;;
    *)
        usage; exit 1
    ;;
esac

shift $(($OPTIND - 1))
if [ $# -lt 1 ] ; then
	usage
	print ERROR: specify a module to test !
	exit 1
fi
moduleToTest=$1
echo "> Module testé: $moduleToTest"


prog=${prog_prfx}${moduleToTest}

echo "> Lancement programme $prog"
echo "> mode rollback: $mode_rollback"
echo "> test du module $moduleToTest en $plateforme"
echo "> log sous $logPath"
echo "> dateFormat: ${dateFormat}"

fileGdb="debug_test.gdb"
fileLog="test_${moduleToTest}.log"

# fabrication du fichier de debugging
cat > ${fileGdb} << EOF0
directory $HOME/projets/bfrance/c/test_unit
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
EOF0



# malloc un peu plus sécurisé
echo "Pour controler les pb memoire à chaque malloc: MALLOC_CHECK_="${MALLOC_CHECK_}
export MALLOC_CHECK_=1


# effacement du log
> ${fileLog}
gdb --tui -x ./${fileGdb} ${progPath}/${prog}
