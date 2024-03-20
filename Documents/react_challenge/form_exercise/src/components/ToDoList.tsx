import { useState,useEffect } from "react";

import { categoryState, toDoSelector,categoriesArr, ICategory, toDoState} from "../atoms";
import CreateToDo from "./CreateToDo";
import ToDo from "./ToDo";
import { useRecoilState, useRecoilValue } from "recoil";

function ToDoList(){
    const selectedArr =useRecoilValue(toDoSelector);
    const [categoryInfo,setCategory]=useRecoilState(categoryState);//현재 카테고리 상태
    const [fullArr,setFullArr]=useRecoilState(toDoState);
    
    const onInput=(event:React.FormEvent<HTMLSelectElement>)=>{
        setCategory(event.currentTarget.value as any);
    }

    const [categoryFromUser,setCategoryFromUser]=useState("");
    const [catArr,setCatArr]=useRecoilState(categoriesArr);//직접 배열에다가 저장할게

    /*useEffect(()=>{
        const storedCategoryInfo = localStorage.getItem("categoryInfo");
        if(storedCategoryInfo){
            setCategory(JSON.parse(storedCategoryInfo));
        }
        const storedCatArr=localStorage.getItem("catArr");
        if(storedCatArr){
            setCatArr(JSON.parse(storedCatArr));
        }
    },[setCatArr]);*/

    useEffect(()=>{
        const storedList=localStorage.getItem("fullArr");
        if(storedList){
            setFullArr(JSON.parse(storedList));
        }
    },[])

    const onSubmit=(event: React.FormEvent<HTMLFormElement>)=>{//일단 제출이 되잖아?
        event.preventDefault();
        setCategoryFromUser("");
        setCatArr((prev=>{
            const newCatArr=[...prev,categoryFromUser];
            localStorage.setItem("catArr",JSON.stringify(newCatArr));
            return newCatArr;
        }))
    }

    useEffect(()=>{
        localStorage.setItem("fullArr",JSON.stringify(fullArr));
    },[fullArr])
    console.log(fullArr);

    useEffect(()=>{//변경된 catInfo정보를 스토리지에 저장
        localStorage.setItem("categoryInfo",JSON.stringify(categoryInfo));
    },[categoryInfo]);

    return (
        <>
        <div>
            <h1>To Dos</h1>
            <hr/>
        <select value={categoryInfo} onInput={onInput}>
            {catArr.map(cat=><option>{cat}</option>)}
        </select>
            <form onSubmit={onSubmit}>
                <input type="text" value={categoryFromUser} onChange={(e)=>{setCategoryFromUser(e.target.value)}}></input>
            </form>
            <CreateToDo/>
            {selectedArr.map(item=><ToDo key={item.id} {...item}/>)}
        </div>
        </>
    );
    }
export default ToDoList;