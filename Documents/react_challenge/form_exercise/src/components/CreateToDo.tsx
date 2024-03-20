import {  useRecoilValue, useSetRecoilState } from "recoil";
import {useForm} from "react-hook-form";
import { categoryState, toDoState } from "../atoms";
import { useEffect } from "react";

export interface IForm{
    toDo:string;
}

function CreateToDo(){//폼을 만드는 컴포넌트
    const {register,handleSubmit,setValue}=useForm<IForm>();
    const setToDosArray=useSetRecoilState(toDoState);
    const toDosArray=useRecoilValue(toDoState);

    const categoryInfo =useRecoilValue(categoryState);
    

    useEffect(()=>{
        const storedList=localStorage.getItem("toDosArray");
        if(storedList){
            setToDosArray(JSON.parse(storedList));
        }
    },[setToDosArray])

    const onValid=({toDo}:IForm)=>{
            //console.log("add to do",toDo);
            const newToDo = { text: toDo, category: categoryInfo, id: Date.now() };
            setToDosArray(prev=>[newToDo,...prev]);
            setValue("toDo","");

                // Update local storage
            const updatedToDos = JSON.stringify([newToDo, ...toDosArray]);
            localStorage.setItem("toDosArray", updatedToDos);
    }  
    console.log(toDosArray);


    return (
        <form onSubmit={handleSubmit(onValid)}>
            <input  {...register("toDo",{required:"Please write your todo"})} placeholder="Write a to do"/>
            <button>ADD</button>
        </form>
    );
}
export default CreateToDo;