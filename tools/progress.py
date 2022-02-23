#!/usr/bin/env python3

__help__ = """Print stats of overall project progress

# Usage: find src/ -name "*.c" | xargs ./tools/progress.py
"""

import argparse
import sys


def main():
    parser = argparse.ArgumentParser(description=__help__, allow_abbrev=False)
    parser.add_argument("files", metavar="FILE", nargs="+", help="sources")
    parser.add_argument("--sort", dest="sortkey", choices=["name", "percent", "completed", "total"], default="percent")
    args = parser.parse_args()

    files = []

    total_funcs = 0
    stubbed_funcs = 0
    unimplemented_funcs = 0

    for file_name in args.files:
        
        # we don't care about harness code
        if 'harness' in file_name:
            continue
            
        f = open(file_name, 'r')

        in_function = False

        file_total = 0
        file_stubbed = 0
        file_unimplemented = 0

        while True:
            line = f.readline()
            if line == '':
                break

            if line[0].isalpha() and '(' in line and line[-2] == '{':
                in_function = True
                file_total = file_total + 1
            if line[0] == '}' and in_function == True:
                # leaving function
                in_function = False

            if in_function:
                if line.strip() == 'NOT_IMPLEMENTED();':
                    file_unimplemented = file_unimplemented + 1
                elif line.strip() == 'STUB();':
                    file_stubbed = file_stubbed + 1
                elif line.strip() == 'STUB_ONCE();':
                    file_stubbed = file_stubbed + 1

        
        if file_total > 0:    
            completed = file_total - (file_stubbed + file_unimplemented)
            file = {
                'name': file_name,
                'percent': round((completed / file_total) * 100),
                'completed': completed,
                'total': file_total,
            }
            files.append(file)
        # if file_total > 0:    
        #     total_funcs = total_funcs + file_total
        #     stubbed_funcs = stubbed_funcs + file_stubbed
        #     unimplemented_funcs = unimplemented_funcs + file_unimplemented

        #     file_completed = 
        #     file_percent_completed = round((file_completed / file_total) * 100)

        #     print('{0}\t\t\t{1}%\t{2} / {3}'.format(file_name, file_percent_completed, file_completed, file_total))
        #     #print(file_name, 'percent', str(file_percent_completed) + '%', 'total', file_total, 'stubbed', file_stubbed, 'not implemented', file_unimplemented)

    #for gv in sorted(module['global_vars'], key=lambda x: x['addr_decimal']):
    print('{0:45} {1:5}\t{2}'.format('name', 'percent', 'completed / total'))
    print("=======================================")

    completed_funcs = 0
    total_funcs = 0
    for f in sorted(files, key=lambda x: x[args.sortkey], reverse=True):
        print('{0:45} {1:5}%\t{2:>4} / {3:>4}'.format(f['name'], f['percent'], f['completed'], f['total']))

        total_funcs = total_funcs + f['total']
        completed_funcs = completed_funcs + f['completed']


    print('{0:45} {1:5}\t{2:>4}'.format('name', 'percent', 'completed / total'))
    print("=======================================")
    percent_completed = round((completed_funcs / total_funcs) * 100)
    print('{0:45} {1:5}%\t{2:>4} / {3:>4}'.format('total', percent_completed, completed_funcs, total_funcs))

    print()
    print('Overall progress:')
    print('{0}% of all functions implemented. ({1} completed / {2} total)'.format(percent_completed, completed_funcs, total_funcs))

    return 0

if __name__ == "__main__":
    raise SystemExit(main())
