This part of the package comes from the following:

```bash
git clone git@github.com:jbsauvan/RegressionTraining.git
```

with the following modifications due to updates of ROOT and TMVA:

In the file ```TMVAMaker.h``` add at the proper place the following two lines: ```#include <TMVA/DataLoader.h>``` and ```TMVA::DataLoader* m_dataloader;```

In the file ```TMVAMaker.cpp``` modify as follows:

ADD
```C++
if(m_dataloader) 
    {
        //m_factory->Delete();
        delete m_dataloader;
        m_dataloader = NULL;
    }
```

REPLACE 
```C++
m_factory = new TMVA::Factory(name.c_str(), m_fileOut, options.c_str());
m_factory->AddRegressionTree(m_tree);
```
BY
```
m_dataloader = new TMVA::DataLoader(name.c_str());
m_dataloader->AddRegressionTree(m_tree);

m_factory = new TMVA::Factory(name.c_str(), m_fileOut, options.c_str());
```

REPLACE
```C++
if(!m_factory)
{
    cout<<"ERROR: TMVAMaker::addVariable(): Cannot add variable: factory doesn't exist\n";
    return;
}
m_factory->AddVariable(name.c_str());
```
BY
```C++
if(!m_dataloader)
{
    cout<<"ERROR: TMVAMaker::addVariable(): Cannot add variable: factory doesn't exist\n";
    return;
}
m_dataloader->AddVariable(name.c_str());
```

REPLACE
```C++
if(!m_factory)
    {
        cout<<"ERROR: TMVAMaker::addSpectator(): Cannot add spectator: factory doesn't exist\n";
        return;
    }
    m_factory->AddSpectator(name.c_str());
```
BY
```C++
if(!m_dataloader)
    {
        cout<<"ERROR: TMVAMaker::addSpectator(): Cannot add spectator: factory doesn't exist\n";
        return;
    }
    m_dataloader->AddSpectator(name.c_str());
```

REPLACE
```C++
if(!m_factory)
    {
        cout<<"ERROR: TMVAMaker::addTarget(): Cannot add target: factory doesn't exist\n";
        return;
    }
    m_factory->AddTarget(name.c_str());
```
BY
```C++
if(!m_dataloader)
    {
        cout<<"ERROR: TMVAMaker::addTarget(): Cannot add target: factory doesn't exist\n";
        return;
    }
    m_dataloader->AddTarget(name.c_str());
```

REPLACE
```C++
m_factory->PrepareTrainingAndTestTree(tcut, options.c_str() ); 
```
BY
```C++
m_dataloader->PrepareTrainingAndTestTree(tcut, options.c_str() ); 
```

REPLACE
```C++
m_factory->BookMethod(method.c_str(), m_name.c_str(), options.c_str());
```
BY
```C++
m_factory->BookMethod(m_dataloader, method.c_str(), m_name.c_str(), options.c_str());   
```

REPLACE
```C++
MethodBDT* method = dynamic_cast<MethodBDT*>(m_factory->GetMethod(m_name.c_str()));
```
BY
```C++
MethodBDT* method = dynamic_cast<MethodBDT*>(m_factory->GetMethod(m_name.c_str(),m_name.c_str()));
```