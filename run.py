#!/usr/bin/python -tt
# added lowercase key on disctionary as all examples in SOM are
# with upper case keys
#  P.Skoda 8.6.2015  according to
# http://www.popmartian.com/tipsntricks/2014/11/20/how-to-lower-case-all-dictionary-keys-in-a-complex-python-dictionary/
import sys
import time
import urllib.request, urllib.parse, urllib.error
import re
import os
import shutil
import os.path
import subprocess
import glob
import json
import random
# import pdb
import numpy as np
import pylab as pl
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec
import math
from random import random


def read_data(path):
    return np.loadtxt(path, delimiter=' ')


def rect(A, A2, A3):
    plt.figure(1)
    # plt.imshow(A, interpolation='nearest')
    plt.imshow(A, interpolation='bilinear')
    # plt.imshow(A, interpolation='bicubic')

    # sort in descending order of biggest circles, so all where visible.
    ind = A3[A3[:, 2].argsort()[::-1]]
    plt.scatter(ind[:, 0], ind[:, 1], s=ind[:, 2], c=ind[:, 3], alpha=1.0)
    # plt.scatter(A3[:,0], A3[:,1], s=A3[:,2], c=A3[:,3], alpha=1.0)

    pl.savefig('result/neurons/nnmatrix.png')


def draw(x, y, c):
    umat = read_data('.temp/umatrix.som');
    cl = read_data('.temp/ClustersTrain.som');

    arr = np.zeros((x, y))
    for row in umat:
        arr[row[0]][row[1]] = row[2]

    arr2 = np.zeros((x, y))
    for row in cl:
        arr2[row[1]][row[2]] += 1

    arr3 = np.zeros((x * y * c, 4))
    for row in cl:
        arr3[row[1] + row[2] * x + row[0] * x * y][0] = row[1]
        arr3[row[1] + row[2] * x + row[0] * x * y][1] = row[2]
        arr3[row[1] + row[2] * x + row[0] * x * y][2] += 10
        arr3[row[1] + row[2] * x + row[0] * x * y][3] = row[0]

    rect(arr, arr2, arr3)


def drange(start, stop, step):
    r = start
    while r <= stop:
        yield r
        r += step


def ciara():
    print(15 * '-')


def error(s):
    ciara()
    print("Error: " + s + ".")
    ciara()
    sys.exit(1)


def getCols(f):
    with open(f, "r") as input_file:
        line = input_file.readline()
        return len(line.split(' '))


def getRows(f):
    os.system("wc -l " + f + " > .temp/wcl")
    data = open('.temp/wcl')
    val, comment = data.readline().split(' ')
    data.close()
    return int(val)


def getShufflerCmd(f, rows, cols, is_names, is_clas, names, clas):
    cmd = './bin/shuffler ' + f + " " + str(rows) + " " + str(cols)
    k = 0
    if is_names: k += 1
    if is_clas: k += 2
    cmd += " " + str(k)
    if is_names: cmd += " " + names
    if is_clas: cmd += " " + clas
    return cmd


def startProgramm(cmd):
    print("Command to run:", cmd)
    err = os.system(cmd)
    if err != 0:
        error("bad return value of command: " + cmd)


def startProgrammDummy(cmd):
    # print "Command to run:", cmd
    err = os.system(cmd)
    if err != 0:
        error("bad return value of command: " + cmd)


def renameKeys(x):
    if type(x) is dict:
        for key in list(x.keys()):
            x[key.lower()] = x.pop(key)
            if type(x[key.lower()]) is dict or type(x[key.lower()]) is list:
                x[key.lower()] = renameKeys(x[key.lower()])
    elif type(x) is list:
        for item in x:
            item = renameKeys(item)
    return x


def main():
    PATH = os.path.dirname(os.path.realpath(__file__))
    json_data = open('config.json', 'r')
    #  pdb.set_trace()
    data = json.load(json_data)
    data = renameKeys(data)

    #####NAME############

    exp_name = data['name']

    #####ALGORITHM#######

    algo = data['algorithm']['bmu']
    if algo == "normal":
        iAlg = 0
    elif algo == "prog":
        iAlg = 1
    thread_num = data['algorithm']['threads']

    #####DATA############

    data_paths = data['data']['path']
    file_type = data['data']['file_type']
    delimiter = data['data']['delimiter']
    cols = None
    #####PARAMETERS######

    topology = data['parameters']['topology']
    neigF = data['parameters']['neighborhood_fcion']
    neigR = data['parameters']['neighborhood_radius']
    mapSizeX = data['parameters']['map_size_x']
    mapSizeY = data['parameters']['map_size_y']
    dataRep = data['parameters']['iteration']
    lrF = data['parameters']['learning_rate_fcion']
    learningRate = data['parameters']['learning_rate']
    prob_iter = data['parameters']['probing_iter']

    if iAlg != 1: prob_iter = 0

    if lrF != "linear" and lrF != "inverse_t" and lrF != "exp_decay":
        error(
            "Learning rate function " + lrF + ' does not exist\nchoose: "linear", "inverse_t", or "exp_decay" function! ')

    #####OUTPUT##########

    visual = data['output']['visualization']
    visual = 1 if visual else 0  # aka in c:  visual = visual ? 1 : 0

    errors = data['output']['error']
    errors = 1 if errors else 0

    web = data['output']['web']
    web = 1 if web else 0

    #####OPTIONAL_INFO###

    opt = data.get('optional_info')
    if opt:
        is_names = opt.get('names')
        is_names = 1 if is_names else 0
        names = opt['names_path']

        is_clas = opt.get('classes')
        is_clas = 1 if is_clas else 0
        clas = opt['classes_path']
    else:
        is_names = 0
        names = None
        is_clas = 0
        names = None
    #####EXCEPTIONS######

    if algo == "probing" and lrF != "exp_decay":
        error("The Probing algorithm is supported only with exp_decey learning rate function!")

    if topology == "hexa" and lrF == "exp_decay":
        error("Combination hexa topology and exp_decey is not yet supported!")

    # clean old results
    os.system("rm -rf ./result/")
    os.system("mkdir result")
    # make temp file
    os.system("rm -rf ./.temp/")
    os.system("mkdir .temp")

    files = []

    for path in data_paths:
        if os.path.isdir(path):
            for infile in (glob.glob(path + "/*")):
                files.append(infile)
        else:
            files.append(path)

    print("Processing files: ")
    for f in files:
        print("  " + f)
        if delimiter != ' ':  # convert to " " delimiter
            os.system("sed  -i 's/" + delimiter + "/ /g' " + f)

    if is_clas:  os.system('cp ' + clas + ' .temp/classes.shuffle')
    if is_names: os.system('cp ' + names + ' .temp/names.shuffle')

    rows = 0

    if lrF == "exp_decay":
        # cmd = getShufflerCmd(f,rows,cols,is_names,is_clas,names,clas)
        # startProgramm(cmd)
        for f in files:
            cols = getCols(f)
            rows += getRows(f)
            cmd = 'cat ' + f + ' >> .temp/data.shuffle'  # change to stream.
            startProgrammDummy(cmd)
    else:
        cmd = 'echo "' + str(cols) + '" >> ./temp/data.shuffle'
        for f in files:
            cols = getCols(f)
            rows += getRows(f)
            cmd = 'cat ' + f + ' >> .temp/data.shuffle'
            startProgrammDummy(cmd)

    if visual: os.makedirs('result/neurons')

    if web:
        for i in range(mapSizeX):
            for j in range(mapSizeY):
                os.makedirs('result/neurons/' + str(i) + '_' + str(j))

    PARAM_STR = f + "_" + str(rows) + "_" + str(cols) + "_" + \
                str(mapSizeX) + "_" + str(mapSizeY) + "_" + str(dataRep) + "_" + str(learningRate)

    if lrF == "exp_decay":

        cmd = PATH + '/bin/som ' + str(rows) + " " + str(cols) + " " + \
              str(mapSizeX) + " " + str(mapSizeY) + " " + str(dataRep) + " " + str(learningRate) + " " + str(
            is_clas) + " " + \
              str(thread_num) + " " + str(iAlg) + " " + str(prob_iter)

        startProgramm(cmd)

    else:
        cmd = PATH + "/som_pak-3.1/randinit -xdim " + str(mapSizeX) + " -ydim " + str(mapSizeY) + \
              " -din .temp/data.shuffle -cout .temp/code1.map -topol " + topology + " -neigh " + neigF
        startProgramm(cmd)
        cmd = PATH + "/som_pak-3.1/vsom -din .temp/data.shuffle -cin .temp/code1.map -cout .temp/code1.map -rlen " + \
              str(rows * dataRep) + " -alpha " + str(learningRate) + " -radius " + str(
            neigR) + " -alpha_type " + lrF + " -rand " + \
              str(random.randrange(1, 1000000000))
        startProgramm(cmd)

        cmd = PATH + '/bin/som ' + str(rows) + " " + str(cols) + " " + \
              str(mapSizeX) + " " + str(mapSizeY) + " " + str(is_clas)
        startProgramm(cmd)

    if errors:
        cmd = PATH + '/bin/errorCounter ' + str(rows) + ' ' + str(mapSizeX) + " " + \
              str(mapSizeY) + ' ' + os.getcwd() + "/" + PARAM_STR + ' ' + str(is_clas)
        startProgramm(cmd)

    if visual:
        cmd = PATH + '/bin/neuronsDrawer result/neurons/nnmatrix.png' + ' ' + \
              str(mapSizeX) + " " + str(mapSizeY) + ' ' + str(rows)
        startProgramm(cmd)

        # head, tail = os.path.split( 'result/neuroDraw/' + PARAM_STR + '.png'  )
        # os.system("mkdir -p " + head)

        cmd = 'gnuplot ' + '.temp/neurons.gnuplot'
        # startProgramm(cmd)

        cmd = PATH + '/bin/neuronsDrawer2 result/neurons/umatrix.png' + ' ' + str(mapSizeX) + \
              " " + str(mapSizeY) + ' ' + str(rows)
        startProgramm(cmd)

        # head, tail = os.path.split( 'result/neuroDraw2/' + PARAM_STR + '.png'  )
        # os.system("mkdir -p " + head)

        cmd = 'gnuplot ' + '.temp/neurons.gnuplot'
        # startProgramm(cmd)

        draw(mapSizeX, mapSizeY, 10)  # TODO change to real value of classes from 10.

        if web:
            os.system("mkdir result/vendor")
            os.system("cp -r " + PATH + "/src/web/vendor result/");

            os.system("mkdir result/lib")
            os.system("cp -r " + PATH + "/src/web/lib result/");
            # os.system("cp src/web/display.html result/display.html");
            # os.system("cp " +PATH + "/src/web/style.css result/style.css");

            cmd = PATH + '/bin/divider ' + str(rows) + " " + str(cols) + " " + \
                  str(mapSizeX) + " " + str(mapSizeY) + " " + str(is_names) + " " + \
                  exp_name
            startProgramm(cmd)

    os.system("rm -rf ./.temp")
    json_data.close()


if __name__ == '__main__':
    main()
