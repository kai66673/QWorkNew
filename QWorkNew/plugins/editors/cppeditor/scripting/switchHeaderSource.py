from PythonQt import QtCore
from QwDocumentManager import *
from QwProjectManager import *

document = documentManager.activeDocument()
if document:
    if not document.isNew():
        fileInfo = QtCore.QFileInfo(document.filePath())
        fileExt = fileInfo.suffix().upper()
        sourceExtensions = ['C', 'C++', 'SC', 'CPP', 'CXX', 'CWW', 'CC']
        headerExtensions = ['H', 'H++', 'HH', 'HP', 'HPP', 'HXX']
        supported = False
        searchExtensions = sourceExtensions
        if sourceExtensions.count(fileExt) > 0:
            supported = True
            searchExtensions = headerExtensions
        elif headerExtensions.count(fileExt) > 0:
            supported = True
        if supported:
            dir = QtCore.QDir(fileInfo.absolutePath())
            wc = [fileInfo.completeBaseName() + '.' + ext for ext in searchExtensions]
            fl = dir.entryList(wc, QtCore.QDir.Files)
            if len(fl) > 0:
                documentManager.openDocument(fileInfo.absolutePath() + '/' + fl[0])
            else:
               if projectManager.isProjectOpened():
                   files = projectManager.projectFiles()
                   for f in files:
                      prFileInfo = QtCore.QFileInfo(f)
                      fileExt = prFileInfo.suffix().upper()
                      if searchExtensions.count(fileExt) > 0:
                          if fileInfo.completeBaseName().upper() == prFileInfo.completeBaseName().upper():
                              documentManager.openDocument(f)
                              break
