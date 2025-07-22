# SVN Commands

## To clone repository into file

Replace \<username\> with your UM username. 

```
svn co https://<username>@cce2014-ict.research.um.edu.mt/svn/CCE2014/2023-2024/Group_05
```

Will ask you for UM password after.

## To udate the repository (pull)

```
svn up
```

Will ask you for UM password after.

## To copy agenda/minutes

Change the XX to the corresponding week

Agenda
```
svn copy agenda_template.tex agenda_weekXX.tex
```

Minutes
```
svn copy minutes_template.tex minutes_weekXX.tex
```

## Tracking files

```
svn add <file-name>
```

Do this for unversioned files, replacing \<filename\>.

You can check unversioned files using ```svn st``` and looking for files with a ```?``` at the start

Files starting with ```A``` were added to the repository.

Files starting with ```M``` were modified.

## Discarding all the changes you did since last pull

If you fuck up royally and want to discard anything you worked on in a file.

```
svn revert <filename>
```

## Deleting file locally and in repository

For this you can also paste in the url of the file in the repository without having to actually pull the file which is neat but not useful.

```
svn delete <filename>
```

## To commit to repo

```
svn commit --message "<message>"
```

Will ask for password after

## If stuck

Pray to god or ```svn help``` and then ```svn help <command>``` a specific command.