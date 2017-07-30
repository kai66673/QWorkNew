import traceback

import rope.contrib.codeassist
import rope.base.project
import rope.base.exceptions
import rope.base.pycore

_codeAssistProject = None

def init_code_assist(projectPath, stubsPath):
    global _codeAssistProject
    _codeAssistProject = rope.base.project.Project(projectPath)
    _codeAssistProject.get_prefs().add('python_path', stubsPath)
    files = _codeAssistProject.get_files()
    for res_file in files:
        _codeAssistProject.pycore.analyze_module(res_file)

def free_code_assist():
    if _codeAssistProject:
        _codeAssistProject.close()

def _type_suffix(o):
    t = o.type
    s = o.scope
    if t == 'instance':
        return '?i'
    elif t == 'class':
        return '?c'
    elif t == 'function':
        return '?f'
    elif t == 'module':
        return '?m'
    elif s == 'keyword':
        return '?k'
    return '?u'

def _get_resource_for_script_name(scriptName):
    if _codeAssistProject is None or scriptName is None:
        return None
    files = _codeAssistProject.get_files()
    resource = None
    for res_file in files:
        if res_file.path == scriptName:
            resource = res_file
            break
    if not resource:
        resource = _codeAssistProject.get_file(scriptName)
    return resource

def handle_script_source_changed(scriptName, source):
    global _codeAssistProject
    if _codeAssistProject:
        resource = _get_resource_for_script_name(scriptName)
        if resource:
            #! Hook For simulate rope-observing
            resource.write(source)
            _codeAssistProject.pycore.object_info._resource_moved(resource, resource)
            _codeAssistProject.pycore.analyze_module(resource)

def get_completion_list(source, position, scriptName = None):
    global _codeAssistProject
    if _codeAssistProject == None:
        return []
    try:
        results = rope.contrib.codeassist.code_assist( _codeAssistProject,
                                                       source, position,
                                                       _get_resource_for_script_name(scriptName) )
    except rope.base.exceptions.ModuleSyntaxError as err:
        print ('code_assist error:\n', err.message)
        return []
    except rope.base.exceptions.BadIdentifierError as err:
        print ('Bad Identifier Error:')
        return []
    except:
        print (traceback.format_exc())
        return []
    completions = [s.name + _type_suffix(s) for s in results]
    return completions
