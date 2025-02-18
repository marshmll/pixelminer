#!/bin/bash

languages=(pt_BR es_ES fr_FR)

for lang in ${languages[@]}; do
    echo "Generating locales for ${lang}"

    if [ ! -d "src/.pixelminer/Locales/${lang}.UTF-8/LC_MESSAGES" ]
    then
        mkdir -p "src/.pixelminer/Locales/${lang}.UTF-8/LC_MESSAGES"
    fi

    # Extract strings from source files and generate the .pot file
    find src include -type f -regex '.*\.\(cxx\|hxx\|cc\|c\|h\)' | \
    xargs xgettext --keyword="_" --from-code=utf-8 -d "pixelminer" --output="src/.pixelminer/Locales/pixelminer.pot" --package-name="pixelminer" --package-version="1.0" --msgid-bugs-address="renandasilvaoliveiraandrade@gmail.com"

    # Check if the .po files exists
    if [ ! -f "src/.pixelminer/Locales/${lang}.UTF-8/LC_MESSAGES/pixelminer.po" ]
    then
        msginit --input="src/.pixelminer/Locales/pixelminer.pot" --output-file="src/.pixelminer/Locales/${lang}.UTF-8/LC_MESSAGES/pixelminer.po" --locale="${lang}.UTF-8" --no-translator
    else
        msgmerge --update "src/.pixelminer/Locales/${lang}.UTF-8/LC_MESSAGES/pixelminer.po" "src/.pixelminer/Locales/pixelminer.pot" --backup=off
    fi

    # Ensure the .po file uses UTF-8 encoding
    sed -i 's/charset=ASCII/charset=UTF-8/' "src/.pixelminer/Locales/${lang}.UTF-8/LC_MESSAGES/pixelminer.po"

    # Compile the .po file into a .mo file
    msgfmt "src/.pixelminer/Locales/${lang}.UTF-8/LC_MESSAGES/pixelminer.po" --output-file="src/.pixelminer/Locales/${lang}.UTF-8/LC_MESSAGES/pixelminer.mo" --check
done