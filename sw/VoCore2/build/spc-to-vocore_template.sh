#
# author        Oliver Blaser
# date          04.01.2022
# copyright     GNU GPLv3 - Copyright (c) 2022 Oliver Blaser
#

user="" # username
host="" # hostname or IP address
destDir="~/led-clock/"

scp ./led-clock $user@$host:$destDir
