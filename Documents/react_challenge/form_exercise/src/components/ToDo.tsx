
import { useRecoilValue, useSetRecoilState} from "recoil";
import { IToDoProps, toDoState,categoriesArr } from "../atoms";
import { useEffect } from "react";

function ToDo({text,category,id}:IToDoProps){//받은 ToDoState를 배열로 나타내는 컴포넌트
    const setToDosArray = useSetRecoilState(toDoState);
    const catArr=useRecoilValue(categoriesArr);
    const fullArr=useRecoilValue(toDoState);

    useEffect(()=>{
        const storedToDoList=localStorage.getItem("fullArr");
        if(storedToDoList){
            setToDosArray(JSON.parse(storedToDoList));
        }
    },[setToDosArray]);

    const onClick=(newCategory:IToDoProps["category"])=>{//parameter로 category를 받는다.
        //버튼을 클릭하면 카테고리가 바뀌어야 한다.
        setToDosArray(prev =>{
            const targetIndex=prev.findIndex(toDo=>toDo.id===id);
            const newToDo={text,category:newCategory,id};
            const updateArr=[...prev.slice(0,targetIndex),newToDo,...prev.slice(targetIndex+1)];
            localStorage.setItem("fullArr",JSON.stringify(updateArr));
            return updateArr;
        })
    } 

    return (
        <li>
            <span>{text}</span> 
            {catArr.map((cat,index:any)=>(category!== catArr[index]&& 
            <button key={index} onClick={()=> onClick(cat)}>{cat}</button>)    
            )}
        </li>
    );
} 
export default ToDo;

//<button key={index} onClick={()=> onClick(cat.category)}>{cat.category}</button>//