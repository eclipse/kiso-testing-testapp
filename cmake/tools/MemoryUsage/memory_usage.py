#!/usr/bin/env python3

import glob
import subprocess
import re
import sys
import os
import argparse
from pathlib import Path

class Function:
    """Data Class for a Function inside a Module"""

    name = ""
    flash_size = 0
    ram_size = 0
    exram_size = 0
    module = ""

    def __init__(self, name: str):
        self.name = name
        self.flash_size = 0
        self.ram_size = 0

    def __repr__(self):
        val = str(
            "["
            + self.name
            + " in "
            + self.module
            + " Flash: "
            + self.flash_size
            + " B RAM: "
            + self.ram_size
            + " B]"
        )
        return val

    def __str__(self):
        val = str(
            "["
            + self.name
            + " in "
            + self.module
            + " Flash: "
            + self.flash_size
            + " B RAM: "
            + self.ram_size
            + " B]"
        )
        return val


class Module:
    name = ""
    libraries = []
    functions = []
    task_name = "common"
    flash_size = 0
    ram_size = 0

    def __init__(self, name: str):
        self.name = name
        self.libraries = []
        self.functions = []
        self.task_name = "common"
        self.flash_size = 0
        self.ram_size = 0

    def __repr__(self):
        val = str(
            self.name
            + "@"
            + self.task_name
            + " libs: "
            + str(self.libraries)
            + "\nFlash Size: "
            + str(self.flash_size)
            + " Bytes\nRAM Size: "
            + str(self.ram_size)
            + " Bytes"
        )
        return val

    def __str__(self):
        val = (
            self.name
            + "@"
            + self.task_name
            + " libs: "
            + str(self.libraries)
            + "\nFlash Size: "
            + str(self.flash_size)
            + " Bytes\nRAM Size: "
            + str(self.ram_size)
            + " Bytes"
        )
        return val


class Task:
    name = ""
    modules = []
    exram = 0
    stack = 0

    def __init__(self, name: str):
        self.name = name
        self.modules = []
        self.exram = 0
        self.stack = 0

    def total_flash(self):
        s = 0
        for m in self.modules:
            s += m.flash_size
        return s

    def total_ram(self):
        s = 0
        for m in self.modules:
            s += m.ram_size
        s += self.stack
        return s

    def __repr__(self):
        return "Task: " + self.name


debug = False


def main():
    global debug
    # read_object_files()
    # with open('modules.json', 'r') as fp:
    #     modules = json.load(fp)
    # print_summary(modules)
    parser = argparse.ArgumentParser()
    parser.add_argument("--directory", help="The directories this program looks into. (Default: Current Directory)", nargs="*", required=False, default=["."])
    parser.add_argument("--exclude-folders", help="Exclude these folders from the search. (Default: None)", nargs="*", required=False, default=[])
    parser.add_argument("--exclude-files", help="Ignores files that match the given Regular Expression. (Default: None)", nargs="*", required=False, default=[])
    parser.add_argument("--modules", help="Specify the module names you are interested in. (Default: Everything)", nargs="*", required=False, default=None)
    parser.add_argument("--map-file", help="The map file that needs to be scanned. (Default: None)", required=True, default="")
    parser.add_argument("--html", help="Specifies HTML output filename. (Default: None)")
    parser.add_argument("--csv", help="Specifies CSV output filename. (Default: None)")
    opts = vars(parser.parse_args(sys.argv[1:]))
    if debug:
        for option, values in opts.items():
            print(str(option) + ": " + str(values))
    modules = []
    for dir in opts["directory"]:
        modules.extend(scan_for_module_directories(dir, opts))
    filt_found = False
    try:
        subprocess.run(["arm-none-eabi-c++filt", "--version"], capture_output=True)
        filt_found = True
    except FileNotFoundError:
        pass
    tasks = scan_map_file(opts["map_file"], modules, filt_found)
    total_exram, total_flash, total_ram = get_total_ram_and_flash(
        opts["map_file"])

    if opts.get('html', None):
        print_HTML(opts['html'], tasks, total_ram, total_exram, total_flash)

    if opts.get('csv', None):
        print_csv(opts['csv'], tasks)


def print_csv(filename, tasks):
    used_flash, used_ram, used_exram = get_used_memory(tasks)

    # Create output directory if non-existent
    directory = os.path.dirname(filename)
    if not os.path.isdir(directory):
        os.makedirs(directory)

    with open(filename, "w") as f:
        f.write('"Total Ram", "Total Exram", "Total Flash"\n')
        f.write("{}, {}, {}".format(str(round(used_ram / 1024, ndigits=2)), str(round(used_exram / 1024, ndigits=2)), str(round(used_flash / 1024, ndigits=2))))


def scan_for_module_directories(dir: str, opts: dict):
    global debug
    path = Path(dir)
    if not path.is_dir() or path in opts["exclude_folders"]:
        return
    src = path.joinpath("src")
    externals = path.joinpath("external")
    modules = scan_dir_for_modules(path, opts)
    if src.exists():
        modules.extend(scan_dir_for_modules(src, opts))
    if externals.exists():
        if debug:
            print("Externals found.")
        modules.extend(scan_for_module_directories(externals, opts))
    return modules


def scan_dir_for_modules(path: Path, opts: dict):
    global debug
    if debug:
        print(path.name)
    modules = []
    mods = [x for x in path.iterdir() if (
        x.is_dir() and x.name != "CMakeFiles")]
    for m in mods:
        if opts["modules"] is not None:
            if m.name.lower() not in map(lambda x: x.lower(), opts["modules"]):
                continue
        new = Module(m.name)
        libs = [
            x.name for x in Path(m).iterdir() if x.is_file() and x.name.endswith(".a")
        ]
        new.libraries = libs.copy()
        for lib in libs:
            for x in opts["exclude_files"]:
                regex = re.compile(x)
                matches = regex.search(lib, re.IGNORECASE)
                if matches:
                    new.libraries.remove(lib)
        if len(new.libraries) != 0:
            modules.append(new)
            if debug:
                print(str(new))
                print("-------------------")
    return modules


def get_total_ram_and_flash(mapfile: str):
    total_ram = 0
    total_exram = 0
    total_flash = 0
    with open(mapfile, "r") as f:
        for line in f:
            matches = re.match(
                r"^(?P<memory>APPLICATION|APPLICATION_DATA_INIT|RAM|PSRAM)\s+0x[0-9a-f]+\s+0x(?P<size>[0-9a-f]+)",
                line,
            )
            if matches:
                val = int(matches.group("size"), base=16)
                if matches.group("memory") in ["APPLICATION", "APPLICATION_DATA_INIT"]:
                    total_flash += val
                elif matches.group("memory") in ["RAM"]:
                    total_ram += val
                elif matches.group("memory") in ["PSRAM"]:
                    total_exram += val
    return total_exram, total_flash, total_ram


def get_used_memory(tasks):
    flash = 0
    ram = 0
    exram = 0
    for t in tasks:
        flash += t.total_flash()
        ram += t.total_ram()
        exram += t.exram
    return (flash, ram, exram)


def scan_map_file(mapfile: str, modules: list, filt_found: bool):
    correctSection = False
    currentTask: Task = None
    currentMemory = ""
    currentFunction = None
    currentLibrary = ""
    currentModule = None
    earlier_match = False
    common_match = False
    prev_addr = None
    privileged_match = False
    tasks = [Task("common")]
    tasks[0].modules.append(Module("Standard Library"))
    with open(mapfile, "r") as f:
        for line in f:
            if correctSection:
                if line.strip() == "/DISCARD/":
                    modules = sort_out_empty_modules(modules, tasks)
                    for m in modules:
                        if m.task_name == "common":
                            tasks[0].modules.append(m)
                    tasks = sort_out_empty_tasks(tasks)
                    return tasks
                if common_match:
                    if "*fill*" in line:
                        continue
                    matches = re.search(r"COMMON\s+0x(?P<address>[0-9a-f]{16}).*(?P<library>lib.*?\.a)", line)
                    if matches:
                        if prev_addr is not None:
                            prev_function.ram_size = int(matches.group('address'), base=16) - prev_addr
                            currentModule.ram_size += prev_function.ram_size
                        currentLibrary = matches.group('library')
                        prev_addr = None
                        prev_function = None
                        continue
                    matches = re.search(r"0x(?P<address>[0-9a-f]{16})\s+(?P<name>[^0-9]\S+)", line)
                    if matches:
                        if prev_addr is not None:
                            prev_function.ram_size = int(matches.group('address'), base=16) - prev_addr
                            currentModule.ram_size += prev_function.ram_size
                        if "__task_section" in matches.group('name'):
                            common_match = False
                            continue
                        prev_addr = int(matches.group('address'), base=16)
                        prev_function = Function(matches.group('name'))
                        for m in modules:
                            if currentLibrary in m.libraries:
                                currentModule = m
                                m.functions.append(prev_function)
                                if currentTask.name != "common" and m not in currentTask.modules:
                                    currentTask.modules.append(m)
                                    m.task_name = currentTask.name
                                break
                        if currentModule is None:
                            currentModule = tasks[0].modules[0]
                    else:  # address and name match
                        common_match = False
                    continue
                matches = re.match(
                    r"^\.privileged_(?P<memory>ram|functions)$", line)
                if matches:
                    currentTask = tasks[0]
                    if matches.group("memory") == "functions":
                        currentMemory = "flash"
                    else:
                        currentMemory = "ram"
                    privileged_match = True
                    continue
                matches = re.match(
                    r"^ *\.task_section_(?P<name>.+?)_(?P<memory>ram|exram|stack|flash)",
                    line,
                )
                if matches:
                    currentTask = find_name_in_tasks(
                        tasks, matches.group("name"))
                    if currentTask is None:
                        currentTask = Task(matches.group("name"))
                        tasks.append(currentTask)
                    currentMemory = matches.group("memory")
                    if currentTask.name == "common" and currentMemory == "flash":
                        currentFunction = None
                    earlier_match = True
                    continue  # set flag to do stuffs.
                matches = re.match(
                    r"^ *\.(?P<memory>text|rodata|init_array)\.", line)
                if matches:
                    currentTask = None
                    currentMemory = matches.group("memory")
                    earlier_match = True
                if earlier_match:
                    privileged_match = False
                    matches = re.search(r"COMMON[ \t]+0x[0-9a-f]+\s+0x[0-9a-f]+[ \t]+.*(?P<library>lib.+?\.a)", line)
                    if matches:
                        currentLibrary = matches.group('library')
                        common_match = True
                        earlier_match = False
                        prev_addr = None
                        prev_function = None
                        continue
                    matches = re.match(
                        r"^[ \t]*\.(text|bss|data|rodata)\.(?P<mangle>\S*)(?P<else>.+)?", line
                    )
                    if matches:
                        mangle = matches.group("mangle").strip().split('.')[0]
                        name = demangle(mangle, filt_found)
                        currentFunction = Function(name)
                        if matches.group("else") is None:
                            continue
                    # regex to find libs
                    # 0xADRESSE   0xLÄNGE     path/to/libName.a
                    matches = re.search(
                        r"0x[0-9a-f]+\s+0x(?P<length>[0-9a-f]+)[ \t]+.*(?P<library>lib.+?\.a)",
                        line,
                    )
                    if currentMemory == "ram":
                        if matches:
                            right_module = tasks[0].modules[0]
                            for m in modules:
                                if matches.group("library") in m.libraries:
                                    # pylint: disable=line-too-long
                                    if (
                                        m not in currentTask.modules
                                        and currentTask.name != "common"
                                    ):  # noqa
                                        currentTask.modules.append(m)
                                        m.task_name = currentTask.name
                                    right_module = m
                                    break
                            # und speichere die LÄNGE in das Module mit der Library
                            if currentFunction is not None:
                                if currentFunction not in right_module.functions:
                                    right_module.functions.append(
                                        currentFunction)
                                currentFunction.ram_size = int(
                                    matches.group("length"), base=16
                                )
                            right_module.ram_size += int(
                                matches.group("length"), base=16)
                            currentFunction = None
                            continue
                    if currentMemory == "stack":
                        if matches:
                            for m in modules:
                                if matches.group("library") in m.libraries:
                                    if (
                                        m not in currentTask.modules
                                        and currentTask.name != "common"
                                    ):  # noqa
                                        currentTask.modules.append(m)
                                        m.task_name = currentTask.name
                                    # und speichere die LÄNGE in das Module mit der Library
                                    currentTask.stack += int(
                                        matches.group("length"), base=16
                                    )
                                    break
                        currentMemory = "ram"
                        continue
                    if currentMemory == "exram":
                        if matches:
                            for m in modules:
                                if matches.group("library") in m.libraries:
                                    if (
                                        m not in currentTask.modules
                                        and currentTask.name != "common"
                                    ):  # noqa
                                        currentTask.modules.append(m)
                                        m.task_name = currentTask.name
                                    # und speichere die LÄNGE in das Module mit der Library
                                    currentTask.exram += int(
                                        matches.group("length"), base=16
                                    )
                                    break
                        continue
                    if currentMemory in ["flash", "text", "rodata", "init_array"]:
                        if matches:
                            right_module = tasks[0].modules[0]
                            for m in modules:
                                if matches.group("library") in m.libraries:
                                    right_module = m
                                    break
                            right_module.flash_size += int(
                                matches.group("length"), base=16
                            )
                            if currentFunction is not None:
                                if currentFunction not in right_module.functions:
                                    right_module.functions.append(
                                        currentFunction)
                                currentFunction.flash_size = int(
                                    matches.group("length"), base=16
                                )
                                currentFunction = None
                        continue
                if privileged_match:
                    matches = re.search(
                        r"0x[0-9a-f]{16}\s+0x(?P<length>[0-9a-f]+)[ \t]+.*"
                        + r"(?P<library>lib.+?\.a)",
                        line,
                    )
                    if matches:
                        if currentMemory in ["flash"]:
                            for m in modules:
                                if matches.group("library") in m.libraries:
                                    m.flash_size += int(
                                        matches.group("length"), base=16
                                    )
                                    break
                        elif currentMemory in ["ram"]:
                            for m in modules:
                                if matches.group("library") in m.libraries:
                                    m.ram_size += int(matches.group("length"), base=16)
                                    break
                    continue
            else:
                if line.strip() == "Linker script and memory map":
                    correctSection = True
    return tasks


def find_name_in_tasks(tasks, name: str):
    for t in tasks:
        if t.name == name:
            return t
    return None


def sort_out_empty_modules(modules, tasks):
    new_mods = modules.copy()
    for m in modules:
        if m.flash_size == 0 and m.ram_size == 0:
            for t in tasks:
                if m in t.modules:
                    t.modules.remove(m)
            new_mods.remove(m)
    return modules


def sort_out_empty_tasks(tasks):
    new_tasks = tasks.copy()
    for t in tasks:
        if len(t.modules) == 0:
            new_tasks.remove(t)
    return new_tasks


def sort_task(val):
    return val.name


def sort_modules(val):
    return val.name


def print_HTML(filename, tasks, total_ram, total_exram, total_flash):
    """Jenkins has a nice way to present HTML files, so I chose this over XML"""
    # Pre-Sorting tasks and modules by Name
    tasks.sort(key=sort_task)
    for t in tasks:
        t.modules.sort(key=sort_modules)
        for m in t.modules:
            m.functions.sort(key=sort_modules)
    used_flash, used_ram, used_exram = get_used_memory(tasks)

    # Create output directory if non-existent
    directory = os.path.dirname(filename)
    if not os.path.isdir(directory):
        os.makedirs(directory)

    with open(filename, "w") as f:
        # Write HTML Header with CSS Styles. Goal is to have exactly one .HTML file, to make it presentable and compact.
        print(
            r"""<!DOCTYPE html>
<html>

<head>
    <meta charset='utf-8'>
    <meta http-equiv='X-UA-Compatible' content='IE=edge'>
    <title> Memory Usage</title>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <!-- there probably is a more compact way to squash the styles, but I didn't bother -->
    <style>
        h1 {
            background-color: #E3000D;
            color: white;
            padding: 18px;
            border: none;
            text-align: left;
            outline: none;
            font-size: larger;
            font-family: Arial, Helvetica, sans-serif;
        }

        .toggler_1 {
            background-color: #F0F0F0;
            color: black;
            cursor: pointer;
            padding: 18px;
            width: 100%;
            border: none;
            text-align: left;
            outline: none;
            font-size: 15px;
        }

        .active_1 .toggler_1,
        .toggler_1:hover {
            background-color: #808080;
        }

        .content {
            table-layout: fixed;
            margin-top: 5px auto;
            margin-bottom: 5px auto;
            font-family: 'Roboto Mono', 'Oxygen Mono', 'Ubuntu Mono', monospace;
            align-self: center;
            width: 100%;
            border-collapse: collapse;
            background-color: #F0F0F0;
            color: black;
        }

        th {
            text-align: center;
        }

        tr {
            text-align: right;
        }

        th,
        td {
            min-width: 33%;
            border: solid 1px;
            border-color: white;
        }

        th:hover,
        th.active {
            cursor: pointer;
            background-color: #808080;
        }

        th.active:not(reversed):after {
            content: '\25B2';
        }

        th.active.reversed:after {
            content: '\25bc'
        }


        span.left {
            text-align: left;
            padding: 5px
        }

        span.right {
            text-align: right;
            float: right;
            font-family: 'Roboto Mono', 'Oxygen Mono', 'Ubuntu Mono', monospace;
        }

        tr.parent:hover, tr.parent.active_2 {
            cursor: pointer;
            background: #808080;
        }

        section.accordion:not(.active_1) .content {
            display: none;
        }

        .child:not(.active_2) {
            display: none;
        }

        .child {
            background: #C8C8C8;
        }

        .child:hover {
            background: #808080;
        }

        td.name {
            text-align: left;
            overflow-x: auto;
        }

        col {
            max-width: initial;
        }

        col.fixed {
            width: 12em;
        }

        #Selector {
            width: 100%;
            text-align: center;
            font-family: -Roboto, apple-system, BlinkMacSystemFont, 'Segoe UI', Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
            color: black;
            margin-top: 5px;
            margin-bottom: 5px;
        }
    </style>
</head>

<body>
    <h1>TestApp Memory Usage<span class=right> RAM: """
            + str(round(used_ram / 1024, ndigits=2))
            + " KB ("
            + "{0:05.2f}".format((used_ram / float(total_ram)) * 100)
            + "%) Flash: "
            + str(round(used_flash / 1024, ndigits=2))
            + " KB ("
            + "{0:05.2f}".format((used_flash / float(total_flash)) * 100)
            + "%) EXRAM: "
            + str(round(used_exram / 1024, ndigits=2))
            + " KB ("
            + "{0:05.2f}".format((used_exram / float(total_exram)) * 100)
            + "%)</span></h1>"
            + """
  <section id="Selector">
    <label for="order">Sort by:</label>
    <select id="order" onchange="sort_modules_by_key(this.value)">
      <option value='Name'>Name</option>
      <option value='RAM'>RAM</option>
      <option value='Flash'>Flash</option>
      <option value='EXRAM'>EXRAM</option>
      <option value='Stack'>Stack</option>
    </select>
  </section>
  """,
            file=f,
        )
        # Write the html module toggler for every task
        for t in tasks:
            print(
                """<section class="accordion">
            <button class="toggler_1"><span class="left"><b>"""
                + t.name
                + '</b></span><span class="right">Total RAM: '
                + str(t.total_ram()).rjust(6).replace(" ", "&#8199;")
                + " B ("
                + "{0:05.2f}".format((t.total_ram() / float(total_ram)) * 100)
                + "%) Total Flash: "
                + str(t.total_flash()).rjust(6).replace(" ", "&#8199;")
                + " B ("
                + "{0:05.2f}".format((t.total_flash() / float(total_flash)) * 100)
                + "%) EXRAM: "
                + str(t.exram).rjust(6).replace(" ", "&#8199;")
                + " B ("
                + "{0:05.2f}".format((t.exram / float(total_exram)) * 100)
                + "%) Stack: "
                + str(t.stack).rjust(6).replace(" ", "&#8199;")
                + """ B</span></button>
  <div>
    <table class="content">
      <colgroup>
        <col>
        <col class="fixed">
        <col class="fixed">
      </colgroup>
      <thead>
        <tr>
            <th class="Alpha">Module</th>
            <th class="Numeric">RAM</th>
            <th class="Numeric">Flash</th>
        </tr>
        </thead>
        <tbody>
      """,
                file=f,
            )
            # Write the table for all modules as child item of our toggle buttons.
            for m in t.modules:
                print(
                    """<tr class="parent">
        <td class="name">"""
                    + m.name
                    + """</td>
        <td>"""
                    + str(m.ram_size)
                    + " B ("
                    + "{0:05.2f}".format(m.ram_size / float(total_ram) * 100)
                    + "%)"
                    """</td>
        <td>"""
                    + str(m.flash_size)
                    + " B ("
                    + "{0:05.2f}".format(m.flash_size / float(total_flash) * 100)
                    + "%)"
                    """</td>
      </tr>
      """,
                    file=f,
                )
                # Write all child-rows for each module
                for func in m.functions:
                    print(
                        """<tr class="child">
        <td class="name">"""
                        + func.name
                        + """</td>
        <td>"""
                        + str(func.ram_size)
                        + " B ("
                        + "{0:05.2f}".format(func.ram_size / float(total_ram) * 100)
                        + "%)"
                        + """</td>
        <td>"""
                        + str(func.flash_size)
                        + " B ("
                        + "{0:05.2f}".format(func.flash_size / float(total_flash) * 100)
                        + "%)"
                        + """</td>
    </tr>""",
                        file=f,
                    )
            print(
                """
        </tbody>
    </table>
  </div>
  </section>
""",
                file=f,
            )
        # Embed all Javascript into the HTML to make it more compact.
        print(
            r"""</body>
    <script type="text/javascript">
    // make buttons togglable
    var togglers = document.getElementsByClassName('toggler_1');
    for (var i = 0; i < togglers.length; i++) {
        togglers[i].addEventListener('click', function () {
            this.parentNode.classList.toggle('active_1');
            return false;
        }, false);
    }
    var parents = document.getElementsByClassName('parent');
    for (var i = 0; i < parents.length; i++) {
        parents[i].addEventListener('click', function () {
            if ((" " + this.className + " ").replace(/[\n\t]/g, " ").indexOf(" active_2 ") > -1) {
                this.classList.remove('active_2');
                var elem = this.nextElementSibling;
                while (!(elem === null || ((" " + elem.className + " ").replace(/[\n\t]/g, " ").indexOf(" parent ") > -1))) {
                    elem.classList.remove('active_2');
                    elem = elem.nextElementSibling;
                }
            }
            else {
                this.classList.add('active_2');
                var elem = this.nextElementSibling;
                while (!(elem === null || ((" " + elem.className + " ").replace(/[\n\t]/g, " ").indexOf(" parent ") > -1))) {
                    elem.classList.add('active_2');
                    elem = elem.nextElementSibling;
                }
            }
        }, false);
    }

    var sortable_tables = document.getElementsByClassName("content");

    for (var i = 0; i < sortable_tables.length; i++) {
        var headers = sortable_tables[i].getElementsByTagName("th");
        for (var j = 0; j < headers.length; j++) {
            headers[j].addEventListener("click", function () {
                var table = this.parentNode.parentNode.parentNode;
                sort_table_by_key(table, this.innerText);
                return false;
            }, false);
        }
    }

    function sort_table_by_key(table, key) {
      var headers = table.getElementsByTagName("th");
      var keys = {};
      var reverse = false;
      for (var i = 0; i < headers.length; i++) {
        keys[headers[i].innerText] = {
          column: i,
          class: headers[i].classList[0],
          header: headers[i]
        }
        if(headers[i].innerText === key){
            headers[i].classList.add("active");
            reverse = headers[i].classList.contains('reversed');
        }
        else headers[i].classList.remove('active', 'reversed');
      }
      var tbody = table.getElementsByTagName("tbody")[0];
      var sorttable_data = {};
      var sort_array = [];
      var childRows = {length: 0};
      var currentParentIndex = 0;
      for (var j = 0; j < tbody.rows.length; j++) {
        var row = tbody.rows[j];
        row.id = j
        if (row.classList.contains("parent")) {
          currentParentIndex = row.id;
          childRows[row.id] = [];
          childRows.length++;
          sort_array.push({ id: row.id, value: row.children[keys[key].column].innerHTML });
          sorttable_data[row.id] = row;
        }
        else {
          childRows[currentParentIndex].push({id:row.id, value: row.children[keys[key].column].innerHTML, row: row});
        }
      }
      if (keys[key].class === 'Numeric') {
        sort_array.sort(function (a, b) {
          var num_a = Number(a.value.split(" B")[0]);
          var num_b = Number(b.value.split(" B")[0]);
          return num_b - num_a;
        });
        for(var j = 0; j < sort_array.length; j++) {
          if(childRows[sort_array[j].id].length === 0) {
            break;
          }
          childRows[sort_array[j].id].sort(function(a, b) {
            var num_a = Number(a.value.split(" B")[0]);
            var num_b = Number(b.value.split(" B")[0]);
            return num_b - num_a;
          });
        }
      }
      else {
        sort_array.sort(function (a, b) {
          return a.value.localeCompare(b.value);
        });
        for(var j = 0; j < sort_array.length; j++) {
          if(childRows[sort_array[j].id].length === 0) {
            break;
          }
          childRows[sort_array[j].id].sort(function(a, b) {
            return a.value.localeCompare(b.value);
          });
        }
      }
      if (reverse) {
        sort_array.reverse();
        for(var j = 0; j < sort_array.length; j++) {
            if (childRows[sort_array[j].id].length === 0) break;
            childRows[sort_array[j].id].reverse();
        }
      }
      var new_tbody = document.createElement("tbody");
      for (var i = 0; i < sort_array.length; i++) {
        var k = sort_array[i].id;
        new_tbody.appendChild(sorttable_data[k]);
        for (var j = 0; j < childRows[sorttable_data[k].id].length; j++) {
          var child = childRows[sorttable_data[k].id][j];
          new_tbody.appendChild(child.row);
        }
      }
      keys[key].header.classList.toggle('reversed');
      table.replaceChild(new_tbody, tbody);
    }

    function sort_modules_by_key(key) {
      const columns = {
        "RAM": 0,
        "Flash": 1,
        "EXRAM": 2,
        "Stack": 3
      };
      var nodelist = document.getElementsByClassName("accordion");
      var modules = Array.from(nodelist);
      var sortable_data = []
      for(var j = 0; j < modules.length; j++) {
        modules[j].id = j;
        var sortable_value = "";
        if (key === 'Name') {
          var sortable_value = modules[j].getElementsByTagName("button")[0].innerText.split(" ")[0];
        }
        else {
          var sortable_value = modules[j].getElementsByTagName('button')[0].innerText.match(/\d+ B/g)[columns[key]];
        }
        sortable_data.push({id:j, value:sortable_value});
      }
      var sort_function = function(a, b) {};
      if(key === 'Name') {
        sort_function = function(a, b) {
          return a.value.localeCompare(b.value);
        }
      }
      else {
        sort_function = function(a, b) {
          return Number(b.value.replace(' B', "")) - Number(a.value.replace(' B', ""));
        }
      }
      sortable_data.sort(sort_function);
      var page = document.getElementsByTagName("body")[0];
      var head = page.getElementsByTagName("h1")[0];
      var selector = page.querySelector("#Selector");

      page.appendChild(head);
      page.appendChild(selector);
      for(var j = 0; j < sortable_data.length; j++) {
        page.appendChild(modules[sortable_data[j].id]);
      }
    }
    </script>
</html>""",
            file=f,
        )


def demangle(names, filt_found):
    try:
        import cpp_demangle
        try:
            return cpp_demangle.demangle(names)
        except ValueError:
            return names
    except ImportError:
        if filt_found:
            args = ["arm-none-eabi-c++filt"]
            args.append(names)
            pipe = subprocess.Popen(args, stdin=subprocess.PIPE,
                                    stdout=subprocess.PIPE)
            stdout, _ = pipe.communicate()
            demangled = stdout.decode("utf8")
            return demangled.rstrip(os.linesep)
        else:
            return names


if __name__ == "__main__":
    main()


"""
Module sind Directory Namen innerhalb der -d flags. [X]
Ein Modul kann mehrere libraries haben. [x]
per regex RAM daten von task_sections suchen (s.u.) innerhalb der Region, alle Module in die man findet in Task hineinwerfen
^\\.task_section_(?P<name>.+?)_(?P<region>ram|exram|stack)
Zuerst werden Module angelegt, und deren flash und ram size ausgelesen
Dann werden Task_Sections durchgegangen, Tasks angelegt und die Module den Tasks aufgrund der libraries zugewiesen.
Dann wird nach \\.text\\..* oder \\.data\\..* gesucht und dort dann die libs herausgesucht und die Länge dem Flash(text)- oder Ram(data)-Speicher zugeordnet.
Wenn eine lib zu einem Modul ohne Task zugeordnet gefunden wurde, gehört dieses Modul zur "Common"-Task
gcc -fStackUsage
Wenn RAM task_section, dann überprüf nach
0xADRESSE   0xLÄNGE     path/to/libName.a
und speichere die LÄNGE in das Module mit der Library

Prozentualer Verbrauch der Task vom Gesamtspeicher anzeigen.
Gesamt Speicher findet sich unter "Memory Configuration"

"""
