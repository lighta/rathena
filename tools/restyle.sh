#!/bin/bash
git fetch origin
git reset --hard origin/refactor/rA-cpp-bleeding-edge
git reset --soft jeyra/lighta_style
git reset tools/
git checkout tools/
find src/ -name "*.[ch]" -or -name "*.cpp" | xargs uncrustify -c ~/style_ra/tools/uncrustify.cfg --no-backup
git commit -am "Automatic lighta Update"
git push jeyra lighta_style
