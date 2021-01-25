import os
from argparse import ArgumentParser


def main():
    parser = ArgumentParser(usage="Script helps with renaming files with tests")
    parser.add_argument("start", help="First copied test number. Example: you have got 3 tests inside `in` dir "
                                      "`xxx0.in`,`xxx1.in` and `xxx2.in` so pass value 3 to continue numbering.")
    parser.add_argument("prog", help="Task prefix which will be used while renaming."
                                     " Example: lic1.cpp has prefix `lic`.")
    parser.add_argument("ext", help="File extension and dir name. Preferably `in` or `out`."
                                    " Example: lic1.out has extension `out` and is stored in"
                                    " <source_path>/out directory.")
    parser.add_argument("source_path", help="Path of dir from which files will be copied."
                                            " Example: /Users/mr.bean/Downloads/small_tests/ ")
    parser.add_argument("out_path", help="Path of dir to which files will be copied. "
                                         "Example: /Users/mr.bean/OI28/lic/ ")
    args = parser.parse_args()
    start = args.start
    prog = args.prog
    ext = args.ext
    source = args.source_path
    out = args.out_path
    files = sorted(os.listdir(f"{source}/{ext}/"))
    for f_idx, file_path in enumerate(files):
        print(f'{source}{ext}/{file_path} -> {out}{ext}/{prog}{start + f_idx}.{ext}')
        with open(f'{source}{ext}/{file_path}') as file:
            content = file.read()
        with open(f'{out}{ext}/{prog}{start + f_idx}.{ext}', 'w') as out_file:
            out_file.write(content)


if __name__ == '__main__':
    main()
