LINUX_VER=$(cat /etc/*-release | grep ID_LIKE)
echo $LINUX_VER

DEPENDECIES="$@"
if [ "$LINUX_VER" == "ID_LIKE=debian" ]; then
    for i in $DEPENDECIES; do
        A=$(dpkg -l | grep $i)
        if [ "$A" == "" ]; then
            echo -en "$i [\033[31mFAIL\033[0m]\n"
            exit 42
        else
            echo -en "$i [\033[32m OK\033[0m ]\n"
        fi
    done
else
    if [ "$LINUX_VER" == "ID_LIKE=archlinux" ]; then
        for i in $DEPENDECIES; do
            A=$(pacman -Qi $i)
            if [ "$A" == "error: package '$i' was not found" ]; then
                echo -en "$i [\033[31mFAIL\033[0m]\n"
                exit 42
            else
                echo -en "$i [\033[32m OK\033[0m ]\n"
            fi
        done
    else
        for i in $DEPENDECIES; do
            A=$(yum list installed $i | grep "not found")
            if [ "$A" == "" ]; then
                echo -en "$i [\033[32m OK\033[0m ]\n"
            else
                echo -en "$i [\033[31mFAIL\033[0m]\n"
                exit 42
            fi
        done
    fi
fi