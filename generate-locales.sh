#!/bin/bash

# Ensure the output directory exists
mkdir -p "src/.pixelminer/Locales/pt_BR.UTF-8/LC_MESSAGES"

# Extract strings from source files and generate the .pot file
find src include -type f -regex '.*\.\(cxx\|hxx\|cc\|c\|h\)' | \
xargs xgettext --keyword="_" --from-code=utf-8 -d "pixelminer" --output="src/.pixelminer/Locales/pixelminer.pot" --package-name="pixelminer" --package-version="1.0" --msgid-bugs-address="renandasilvaoliveiraandrade@gmail.com"

# Check if the .po file exists
if [ ! -f "src/.pixelminer/Locales/pt_BR.UTF-8/LC_MESSAGES/pixelminer.po" ]
then
    echo "Creating Portable Object file"
    msginit --input="src/.pixelminer/Locales/pixelminer.pot" --output-file="src/.pixelminer/Locales/pt_BR.UTF-8/LC_MESSAGES/pixelminer.po" --locale="pt_BR.UTF-8" --no-translator
else
    echo "Merging Portable Object file"
    msgmerge --update "src/.pixelminer/Locales/pt_BR.UTF-8/LC_MESSAGES/pixelminer.po" "src/.pixelminer/Locales/pixelminer.pot" --backup=off
fi

# Ensure the .po file uses UTF-8 encoding
sed -i 's/charset=ASCII/charset=UTF-8/' "src/.pixelminer/Locales/pt_BR.UTF-8/LC_MESSAGES/pixelminer.po"

# Compile the .po file into a .mo file
msgfmt "src/.pixelminer/Locales/pt_BR.UTF-8/LC_MESSAGES/pixelminer.po" --output-file="src/.pixelminer/Locales/pt_BR.UTF-8/LC_MESSAGES/pixelminer.mo" --check