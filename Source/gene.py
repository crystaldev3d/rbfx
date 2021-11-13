import sys
import os
import re
from collections import namedtuple

FileInfo = namedtuple('FileInfo', 'full_name, include_name, enums')
EnumInfo = namedtuple('EnumInfo', 'name, begin_line, end_line, original_values, converted_values')


def strip_common_prefix(enum_values):
    if len(enum_values) == 0:
        return enum_values
    index = enum_values[0].find('_')
    if index is None:
        return enum_values
    prefix = enum_values[0][0:index+1]
    new_values = []
    for value in enum_values:
        if not value.startswith(prefix) and not value.startswith('MAX_'):
            return enum_values
        new_values.append(value.replace(prefix, '', 1))
    return new_values


def convert_enum_values(enum_values):
    stripped_values = strip_common_prefix(enum_values)
    return [value.replace('_', ' ').title().replace(' ', '') for value in stripped_values]


def main():
    if len(sys.argv) != 2:
        print('Command line syntax: gene.py path/to/source')
        return

    source_folder = sys.argv[1]
    urho3d_folder = source_folder + '/Urho3D/'

    # Scan for Urho3D source files
    print(f'Scanning {urho3d_folder}...')
    urho3d_files = []
    for subdir, _, files in os.walk(urho3d_folder):
        for file in files:
            full_file_name = subdir + os.sep + file

            if full_file_name.endswith(".h") or full_file_name.endswith(".cpp") or full_file_name.endswith(".hpp"):
                urho3d_files.append(full_file_name)
    print(f'{len(urho3d_files)} files found!')

    # Scan for enums
    enum_regex = re.compile(r'enum\s+([\d\w_]+)')
    enum_value_regex = re.compile(r'^\s*([A-Z0-9_]+)')

    relevant_files = []

    for file_name in urho3d_files:
        file_info = FileInfo(file_name, file_name.replace(urho3d_folder, '').replace('\\', '/'), [])
        with open(file_name) as reader:
            file_lines = reader.readlines()
            i = 0
            while i < len(file_lines):
                # Skip comments
                begin_line_index = i
                i = i + 1
                line = file_lines[begin_line_index]
                if line.startswith('//'):
                    continue

                # Skip non-enums
                enum_name = enum_regex.match(line)
                if enum_name is None:
                    continue
                enum_name = enum_name.group(1)

                # Skip if next line is not {
                if file_lines[begin_line_index + 1].strip() != '{':
                    continue
                begin_line_index = begin_line_index + 2

                # Find end line index
                end_line_index = None
                for j in range(begin_line_index, len(file_lines)):
                    if file_lines[j].strip() == '};':
                        end_line_index = j
                        break
                if end_line_index is None:
                    continue

                enum_values = []
                for j in range(begin_line_index, end_line_index):
                    value = enum_value_regex.match(file_lines[j])
                    if value is None:
                        continue
                    enum_values.append(value.group(1))

                enum_info = EnumInfo(enum_name, begin_line_index, end_line_index, enum_values, convert_enum_values(enum_values))
                file_info.enums.append(enum_info)

        if len(file_info.enums) > 0:
            relevant_files.append(file_info)

    # Patch enum declarations
    print(relevant_files)



main()
