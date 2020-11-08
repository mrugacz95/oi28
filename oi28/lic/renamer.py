import os
from argparse import ArgumentParser


def main():
    start = 5
    prog = 'lic'
    parser = ArgumentParser()
    parser.add_argument('source')
    parser.add_argument('out')
    args = parser.parse_args()
    files = os.listdir(args.source)
    for f_idx, file_path in enumerate(files):
        print(f'{args.source}{file_path}', " -> ", args.out + f'{prog}{start + f_idx}')
        with open(f'{args.source}{file_path}') as file:
            content = file.read()
        with open(args.out + f'{prog}{start + f_idx}.out', 'w') as out_file:
            out_file.write(content)


if __name__ == '__main__':
    main()